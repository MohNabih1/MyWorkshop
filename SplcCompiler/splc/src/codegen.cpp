#include <algorithm>
#include "Node.h"
#include "AST.h"

extern bool trace_codegen;
extern int tabs;

Type expand_type ( Type tp );
Type typecheck ( Expr e );

/** symbol table to store declarations */
extern Environment<Declaration>* st;

/** global declarations */
Environment<IRdecl>* declarations = new (GC) Environment<IRdecl>();

string new_name ( const string &name ) {
  static int name_counter = 0;
  return name+"_"+to_string(name_counter++);
}

void normalize ( IRstmt e, vector<IRstmt> &stmts );

/* convert the IR exp into a list of IRs that do not contain any Seq or ESeq IRs */
IRexp normalize ( IRexp e, vector<IRstmt> &stmts ) {
  if (e->isMem())
    return mkMem(normalize(e->toMem()->address,stmts));
  else if (e->isIndexed()) {
    auto z = e->toIndexed();
    return mkIndexed(normalize(z->address,stmts),normalize(z->offset,stmts));
  } else if (e->isBinop()) {
    auto z = e->toBinop();
    return mkBinop(z->op,normalize(z->left,stmts),normalize(z->right,stmts));
  } else if (e->isUnop()) {
    auto z = e->toUnop();
    return mkUnop(z->op,normalize(z->operand,stmts));
  } else if (e->isCall()) {
    auto z = e->toCall();
    vector<IRexp> s;
    for ( auto x: z->arguments )
      s.push_back(normalize(x,stmts));
    return mkCall(z->address,z->static_link,s);
  } else if (e->isESeq()) {
    auto z = e->toESeq();
    normalize(z->stmt,stmts);
    return normalize(z->value,stmts);
  } else if (e->isAllocate()) {
    auto z = e->toAllocate();
    return mkAllocate(z->otype,normalize(z->size,stmts));
  } else if (e->isCoerce()) {
    auto z = e->toCoerce();
    return mkCoerce(normalize(z->value,stmts),z->from_type,z->to_type);
  } else return e;
}

void normalize ( IRstmt e, vector<IRstmt> &stmts ) {
  if (e->isMove()) {
    auto z = e->toMove();
    auto d = normalize(z->destination,stmts);
    auto s = normalize(z->source,stmts);
    stmts.push_back(mkMove(d,s));
  } else if (e->isCJump()) {
    auto z = e->toCJump();
    stmts.push_back(mkCJump(normalize(z->condition,stmts),z->label));
  } else if (e->isSeq()) {
    for ( auto s: e->toSeq()->stmts )
      normalize(s,stmts);
  } else if (e->isCallP()) {
    auto z = e->toCallP();
    vector<IRexp> s;
    for ( auto x: z->arguments )
      s.push_back(normalize(x,stmts));
    stmts.push_back(mkCallP(z->address,z->static_link,s));
  } else if (e->isSystemCall()) {
    auto z = e->toSystemCall();
    stmts.push_back(mkSystemCall(z->name,normalize(z->arg,stmts)));
  } else if (e->isReturn())
    stmts.push_back(mkReturn(normalize(e->toReturn()->value,stmts)));
  else if (e->isAssert())
    stmts.push_back(mkAssert(normalize(e->toAssert()->condition,stmts)));
  else stmts.push_back(e);
}

/** data allocation in the frames and heap */
Type data_layout ( Type tp ) {
  if (tp->isNamedType()) {
    auto z = tp->toNamedType();
    if (!declarations->exists(z->name)
        || !declarations->lookup(z->name)->isIRTypeDecl())
      splc_error("Undefined type name: "+z->name);
    auto td = declarations->lookup(z->name)->toIRTypeDecl();
    return mkPointerType(mkNamedType(td->label));
  } else if (tp->isArrayType()) {
    // an array is allocated as a tuple (length,array) in the heap
    auto el = data_layout(tp->toArrayType()->element_type);
    return mkPointerType(mkTupleType({ mkIntType(),   // array length
                                       mkArrayType(el) }));
  } else if (tp->isTupleType()) {
    auto z = tp->toTupleType();
    vector<Type> ts;
    for ( auto t: z->components )
      ts.push_back(data_layout(t));
    return mkPointerType(mkTupleType(ts));
  } else if (tp->isRecordType()) {
    auto z = tp->toRecordType();
    vector<Type> ts;
    for ( auto t: z->components )
      ts.push_back(data_layout(get<1>(t)));
    return mkPointerType(mkTupleType(ts));
  } if (tp->isFunctionType()) {
    auto z = tp->toFunctionType();
    auto ts = vector<Type>({ mkPointerType(mkByteType()) });
    for ( auto t: z->formal_params )
      ts.push_back(data_layout(t));
    auto ftp = mkFunctionType(ts,data_layout(z->result_type));
    // a closure is a pair of a function type with a static link type (a void* type)
    auto ctp = vector<Type>({ mkPointerType(ftp),
	                      mkPointerType(mkByteType()) });
    return mkPointerType(mkTupleType(ctp));
  } else return tp;
}

/** allocate a new variable at the end of the current frame and return its value */
IRexp allocate_in_frame ( string name, Type var_type, string fname ) {
  if (!st->exists(fname))
    splc_error("No current function: " + fname);
  auto d = st->lookup(fname);
  if (d->isFuncDeclaration()) {
    auto fd = d->toFuncDeclaration();
    // allocate variable at the next available offset in frame
    st->insert(name,mkVarDeclaration(var_type,fd->level,fd->available_offset));
    // update the next available offset in frame
    st->replace(fname,mkFuncDeclaration(fd->formal_params,fd->result_type,
                              fd->label,fd->level,fd->available_offset+1));
    // return the code that accesses the variable
    return mkMem(mkIndexed(mkFramePointer(),mkIntValue(fd->available_offset)));
  } else splc_error("Not a function: " + fname);
}

/** return the address of a frame-allocated variable from the run-time stack */
IRexp access_variable ( string name, int level, string fname ) {
  if (!st->exists(name))
    splc_error("Undefined variable: " + name);
  auto d = st->lookup(name);
  if (d->isVarDeclaration()) {
    auto vd = d->toVarDeclaration();
    IRexp sl = mkFramePointer();
    // non-local variable: follow the static link (level-var_level) times
    for ( int i = vd->level; i < level; i++ )
      sl = mkMem(mkIndexed(sl,mkIntValue(0)));
    return mkIndexed(sl,mkIntValue(vd->offset));
  } else if (d->isFuncDeclaration()) {
    auto fd = d->toFuncDeclaration();
    IRexp sl = mkFramePointer();
    // non-local variable: follow the static link (level-var_level) times
    for ( int i = fd->level; i < level; i++ )
      sl = mkMem(mkIndexed(sl,mkIntValue(0)));
    vector<Type> ps;
    for ( auto t: fd->formal_params )
      ps.push_back(get<1>(t));
    // construct a closure
    auto ftp = mkFunctionType(ps,fd->result_type);
    auto c = allocate_in_frame("C",ftp,fname);
    return mkESeq(mkMove(c,mkClosure(fd->label,sl)),
                  access_variable("C",level,fname));
  } else splc_error("Not a variable or function: " + name);
}

string toUpper ( const string &s ) {
  auto ns = string(s);
  transform(ns.begin(),ns.end(),ns.begin(),::toupper);
  return ns;
}

IRexp coerce ( IRexp e, Type from_type, Type to_type ) {
  return (*from_type == *to_type) ? e : mkCoerce(e,from_type,to_type);
}

IRstmt code ( Stmt e, int level, const string &fname, const string &exit_label );
IRexp code_ ( Expr e, int level, const string &fname );
IRexp code ( Expr e, int level, const string &fname ) {
  if (trace_codegen)
    cout << string((tabs++)*3,' ') << "** " << e->toString() << endl;
  auto x = code_(e,level,fname);
  if (trace_codegen)
    cout << string((--tabs)*3,' ') << "-> " << x->toString() << endl;
  return x;
}

/** return the IR code from the Expr e (level is the current function nesting level,
 *  fname is the name of the current function/procedure) */
IRexp code_ ( Expr e, int level, const string &fname ) {
    if (e->isBinOpExp()) {
    auto z = e->toBinOpExp();
    auto cl = code(z->left,level,fname);
    auto cr = code(z->right,level,fname);
    // coerce the operands if needed
    if (z->op == "plus" || z->op == "minus" || z->op == "times" | z->op == "div"
        || z->op == "gt" || z->op == "lt" || z->op == "geq" || z->op == "leq") {
      auto tp = typecheck(mkBinOpExp("plus",z->left,z->right));
      cl = coerce(cl,typecheck(z->left),tp);
      cr = coerce(cr,typecheck(z->right),tp);
    }
    auto nop = toUpper(z->op);
    return mkBinop(nop,cl,cr);
  } else if (e->isArrayGen()) {
    auto z = e->toArrayGen();
    auto nv = new_name("index");
    auto nav = new_name("array");
    auto tp = typecheck(e);
    auto etp = typecheck(z->value);
    auto adef = allocate_in_frame(nav,tp,fname);
    auto len = code(z->length,level,fname);
    auto aloc = mkAllocate(data_layout(tp)->toPointerType()->address,
			   mkBinop("PLUS",
                                   mkTypeSize(etp),
                                   mkBinop("TIMES",mkTypeSize(data_layout(etp)),len)));
    return mkESeq(mkSeq(vector<IRstmt>(
                          { mkMove(adef,aloc),
                            mkMove(mkMem(mkIndexed(adef,mkIntValue(0))),len),
                            code(mkForSt(nv,mkIntConst(0),
                                         mkBinOpExp("minus",z->length,mkIntConst(1)),
                                         mkIntConst(1),
                                         mkAssignSt(mkArrayDeref(mkVar(nav),mkVar(nv)),
                                                    z->value)),
                                 level,fname,"") })),
                  adef);
  } else if (e->isFunctionExp()) {
    auto z = e->toFunctionExp();
    auto lname = new_name("lambda");
    /* create a closure for the anonymous function: it contains the
       function address and a pointer to the current frame */
    code(mkFuncDef(lname,z->formal_params,z->result_type,z->body),
         level,fname,"");
    auto fd = st->lookup(lname)->toFuncDeclaration();
    return mkClosure(fd->label,mkFramePointer());
  } 

  /* PUT YOUR CODE HERE */

  else splc_error("Unrecognized expression: "+e->toString());
}

IRstmt code_ ( Stmt e, int level, const string &fname, const string &exit_label );
IRstmt code ( Stmt e, int level, const string &fname, const string &exit_label ) {
  if (trace_codegen)
    cout << string((tabs++)*3,' ') << "** " << e->toString() << endl;
  auto x = code_(e,level,fname,exit_label);
  if (trace_codegen)
    cout << string((--tabs)*3,' ') << "-> " << x->toString() << endl;
  return x;
}

/** return the IR code from the Statement e (level is the current function nesting level,
 *  fname is the name of the current function/procedure)
 *  and exit_label is the exit label       */
IRstmt code_ ( Stmt e, int level, const string &fname, const string &exit_label ) {
  if (e->isForSt()) {
    auto z = e->toForSt();
    auto loop = new_name("Loop");
    auto exit = new_name("Exit");
    auto cv = allocate_in_frame(z->variable,mkIntType(),fname);
    auto ca = code(z->initial,level,fname);
    auto cb = code(z->last,level,fname);
    auto cc = code(z->increment,level,fname);
    auto cs = code(z->body,level,fname,exit);
    return mkSeq(vector<IRstmt>(
                           { mkMove(cv,ca),  // needs cv, not Mem(cv)
                             mkLabel(loop),
                             mkCJump(mkBinop("GT",cv,cb),exit),
                             cs,
                             mkMove(cv,mkBinop("PLUS",cv,cc)),  // needs cv, not Mem(cv)
                             mkJump(loop),
                             mkLabel(exit) }));
  } else if (e->isFuncDef()) {
    auto z = e->toFuncDef();
    auto flabel = (z->name == "main") ? z->name : new_name(z->name);
    st->insert(z->name,
               mkFuncDeclaration(z->formal_params,z->result_type,flabel,level+1,
                                 (z->name == "main") ? 0 : z->formal_params.size()+1));
    st->begin_scope();
    /* allocate formal parameters in the frame */
    auto i = 1;
    for ( auto t: z->formal_params )
      st->insert(get<0>(t),mkVarDeclaration(get<1>(t),level+1,i++));
    /* remove all Seq and ESeq from the function body */
    auto body = vector<IRstmt>();
    auto cbody = code(z->body,level+1,z->name,"");
    normalize(cbody,body);
    /* get the types of all local variables ordered by their offset */
    vector<Type> frame_types;
    if (z->name != "main") {
      auto frame_type_name = st->lookup(fname)->toFuncDeclaration()->label+"_frame";
      frame_types.push_back(mkPointerType(mkNamedType(frame_type_name)));
    }
    for ( auto d: *st->current_scope() )
      if (get<1>(*d)->isVarDeclaration())
        frame_types.push_back(data_layout(get<1>(*d)->toVarDeclaration()->vartype));
    vector<Type> formals;
    for ( auto fp: z->formal_params )
      formals.push_back(data_layout(get<1>(fp)));
    st->end_scope();
    declarations->insert(flabel+"_frame",
                         mkIRTypeDecl(flabel+"_frame",mkTupleType(frame_types)));
    declarations->insert(z->name,
                         mkIRFuncDecl(flabel,formals,data_layout(z->result_type),
                                      frame_types,level+1,body));
    return mkSeq(vector<IRstmt>());
  }

  /* PUT YOUR CODE HERE */

  else splc_error("Unrecognized statement: "+e->toString());
}

/** return the IR code from a program */
list<tuple<string,IRdecl>*>* code ( Program e ) {
  st->begin_scope();
  declarations->begin_scope();
  code(mkFuncDef("main",vector<tuple<string,Type>>(),
                 mkNoType(),e->body),0,"","");
  auto res = declarations->current_scope();
  declarations->end_scope();
  st->end_scope();
  return res;
}
