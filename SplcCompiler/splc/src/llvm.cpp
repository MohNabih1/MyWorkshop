/*
 * Copyright © 2024 Leonidas Fegaras, University of Texas at Arlington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "Node.h"
#include "AST.h"

Type data_layout ( Type tp );

list<tuple<string,IRdecl>*>* ir_declarations;

extern string malloc_op;
extern string spl_home;

ofstream out;

/** contains global LLVM constants (currently strings only) */
vector<string> globals;

/** the type name of the current frame */
string frame_type_name = "";

/** the type of the current frame (a tuple) */
Type frame_type = mkNoType();

/** does LLVM need a jump instruction before any label? */
bool after_jump = false;

/** representation of LLVM value */
typedef string lval;

/** generate a new register name */
int register_count = 1;
lval new_temp_var () {
  return "%R" + to_string(++register_count);
}

/** generate a new label name */
int name_count = 1;
string new_label () {
  return "L_" + to_string(name_count++);
}

template< typename T >
string join ( const vector<T> v, string(*f)(T), const string &sep ) {
  string s = "";
  int i = 0;
  for ( auto x: v ) {
    if (i++ > 0) s += sep;
    s += f(x);
  }
  return s;
}

/** convert an IRtype to an LLVM type */
string ltype ( Type tp ) {
  if (tp->isBooleanType())
    return "i1";
  else if (tp->isByteType())
    return "i8";
  else if (tp->isIntType())
    return "i32";
  else if (tp->isFloatType())
    return "float";
  else if (tp->isNoType())
    return "i8*";
  else if (tp->isStringType())
    return "{i32,i8*}*";   // string length + pointer to string content
  else if (tp->isAnyType())
    return "i8*";
  else if (tp->isNamedType())
    return "%"+tp->toNamedType()->name;
  else if (tp->isPointerType())
    return ltype(tp->toPointerType()->address)+"*";
  else if (tp->isArrayType())
    return "[0 x "+ltype(tp->toArrayType()->element_type)+"]";
  else if (tp->isTupleType())
    return "{ "+join(tp->toTupleType()->components,ltype,", ")+" }";
  else if (tp->isFunctionType()) {
    auto z = tp->toFunctionType();
    return ((z->result_type->isNoType()) ? "void " : ltype(z->result_type))
           + "( "+join(z->formal_params,ltype,", ")+" )";
  } else splc_error("Wrong IRtype: "+tp->toString());
}

/** emit LLVM code */
void llvms ( const string &op ) {
  out << "    " << op << endl;
}

/** emit an LLVM label */
void llvm_label ( const string &s ) {
  if (!after_jump)
    llvms("br label %"+s);
  out << "  " << s << ":\n";
}

/** emit LLVM code and bind the result to a new temp variable */
lval llvm ( const string &op ) {
  auto s = new_temp_var();
  out << "    " << s << " = " << op << endl;
  return s;
}

/** allocate a new string constant */
string allocate_string ( const string &s ) {
  int i = 0;
  for ( auto x: globals ) {
    if (x == s)
      return "S_"+to_string(i);
    i++;
  }
  globals.push_back(s);
  return "S_"+to_string(globals.size()-1);
}

Type expand_named_type ( Type tp ) {
  if (tp->isNamedType()) {
    auto name = tp->toNamedType()->name;
    for ( auto d: *ir_declarations )
      if (get<1>(*d)->isIRTypeDecl() && get<1>(*d)->toIRTypeDecl()->label == name)
        return expand_named_type(get<1>(*d)->toIRTypeDecl()->hasType);
    splc_error("Unknown named type "+name);
  } else return tp;
}

/** return the type of an IRexp */
Type type_of ( IRexp e ) {
  if (e->isIntValue())
    return mkIntType();
  else if (e->isFloatValue())
    return mkFloatType();
  else if (e->isBooleanValue())
    return mkBooleanType();
  else if (e->isStringValue())
    return mkStringType();
  else if (e->isNullValue())
    return mkNoType();
  else if (e->isVoidValue())
    return mkNoType();
  else if (e->isFramePointer())
    return mkPointerType(mkNamedType(frame_type_name));
  else if (e->isAllocate())
    return mkPointerType(e->toAllocate()->otype);
  else if (e->isClosure()) {
    auto z = e->toClosure();
    for ( auto d: *ir_declarations )
      if (get<1>(*d)->isIRFuncDecl()) {
        auto fd = get<1>(*d)->toIRFuncDecl();
        if (fd->label == z->function_name) {
          auto fs = vector<Type>(fd->formal_params);
          fs.insert(fs.begin(),mkPointerType(mkByteType()));
          auto ftp = mkFunctionType(fs,fd->result_type);
          return mkPointerType(mkTupleType(vector<Type>(
                                               { mkPointerType(ftp),
                                                 mkPointerType(mkByteType()) })));
        }
      }
    splc_error("Unknown function: "+z->function_name);
  } else if (e->isMem())
    return type_of(e->toMem()->address)->toPointerType()->address;
  else if (e->isIndexed()) {
    auto z = e->toIndexed();
    auto tp = expand_named_type(type_of(z->address)->toPointerType()->address);
    if (tp->isArrayType())
      return mkPointerType(tp->toArrayType()->element_type);
    else if (tp->isTupleType() && z->offset->isIntValue())
      return mkPointerType(tp->toTupleType()->components[z->offset->toIntValue()->value]);
    else splc_error("Expected array or tuple in indexing "+e->toString()
               +" on type "+tp->toString());
  } else if (e->isBinop()) {
    auto z = e->toBinop();
    if (z->op == "LT" || z->op == "GT" || z->op == "LEQ" || z->op == "GEQ"
        || z->op == "EQ" || z->op == "NEQ" || z->op == "AND" || z->op == "OR")
      return mkBooleanType();
    else return type_of(z->left);
  } else if (e->isUnop())
    return type_of(e->toUnop()->operand);
  else if (e->isAddress()) {
    auto name = e->toAddress()->value;
    for ( auto d: *ir_declarations )
      if (get<1>(*d)->isIRFuncDecl()) {
        auto fd = get<1>(*d)->toIRFuncDecl();
        if (fd->label == name) {
          auto fs = vector<Type>(fd->formal_params);
          fs.insert(fs.begin(),mkPointerType(mkByteType()));
          return mkPointerType(mkFunctionType(fs,fd->result_type));
        }
      }
    splc_error("Unknown address "+name);
  } else if (e->isCall()) {
    auto tp = type_of(e->toCall()->address)->toPointerType()->address;
    if (tp->isFunctionType())
      return tp->toFunctionType()->result_type;
    else if (tp->isTupleType()) {
      auto etp = tp->toTupleType()->components[0];
      if (etp->isPointerType())
        return etp->toPointerType()->address->toFunctionType()->result_type;
    }
    splc_error("Not a function: "+e->toCall()->address->toString());
  } else if (e->isCoerce())
    return e->toCoerce()->to_type;
  else if (e->isTypeSize())
    return mkIntType();
  else splc_error("Cannot get the type of "+e->toString());
}

/** convert the type of an IRexp to an LLVM type */
string llvm_type ( IRexp e ) {
  return ltype(type_of(e));
}

/** generate LLVM code from an IR statement */
void emit ( IRstmt e, Type return_type );

/** generate LLVM code from the IR expression e */
lval emit ( IRexp e ) {
  if (e->isIntValue())
    return to_string(e->toIntValue()->value);
  else if (e->isFloatValue()) {
    // must be converted to a double HEX with the last 29 bits cleared
    double v = (double)e->toFloatValue()->value;
    long* q = (long*)&v;
    stringstream str;
    str << hex << *q;
    return "0x"+str.str();
  } else if (e->isBooleanValue())
    return (e->toBooleanValue()->value) ? "1" : "0";
  else if (e->isStringValue())
    return "@"+allocate_string(e->toStringValue()->value);
  else if (e->isNullValue())
    return "null";
  else if (e->isVoidValue())
    return "null";
  else if (e->isAddress())
    return "@"+e->toAddress()->value;
  else if (e->isFramePointer())
    return "%fp";
  else if (e->isMem()) {
    auto z = e->toMem();
    auto tp = type_of(z->address);
    if (tp->isPointerType()) {
      auto ltp = ltype((Type)tp->toPointerType()->address);
      return llvm("load "+ltp+", "+ltp+"* "+emit(z->address));
    } else splc_error("Wrong address: "+z->address->toString());
  } else if (e->isIndexed()) {
    auto z = e->toIndexed();
    auto tp = type_of(z->address);
    if (tp->isPointerType()) {
      auto ltp = ltype((Type)tp->toPointerType()->address);
      return llvm("getelementptr "+ltp+", "+ltp+"* "+emit(z->address)
                  +", i32 0, i32 "+emit(z->offset));
    } else splc_error("Wrong pointer arithmetic on type "+tp->toString()
                 +" in "+e->toString());
  } else if (e->isClosure()) {
    auto z = e->toClosure();
    auto ctp = type_of(e);
    auto tp = ctp->toPointerType()->address;
    auto g = tp->toTupleType()->components[0];
    auto fttp = g->toPointerType()->address->toFunctionType();
    auto fs = fttp->formal_params;
    auto ot = fttp->result_type;
    auto ltp = ltype((Type)tp);
    auto gtp = ltype((Type)g);
    auto sltp = type_of(z->static_link);
    auto nfs = vector<Type>(fs);
    nfs[0] = sltp;
    auto ftp = ltype(mkFunctionType(nfs,ot));
    auto sz = llvm("zext i32 "+emit(mkTypeSize(tp))+" to i64");
    auto c = llvm("call noalias i8* @"+malloc_op+"(i64 "+sz+")");
    auto closure = llvm("bitcast i8* "+c+" to "+ltp+"*");
    auto c0 = llvm("getelementptr "+ltp+", "+ltp+"* "+closure+", i32 0, i32 0");
    auto ff = llvm("bitcast "+ftp+"* @"+z->function_name+" to "+gtp);
    llvms("store "+gtp+" "+ff+", "+gtp+"* "+c0);
    auto c1 = llvm("getelementptr "+ltp+", "+ltp+"* "+closure+", i32 0, i32 1");
    auto sl = llvm("bitcast "+ltype(sltp)+" "+emit(z->static_link)+" to i8*");
    llvms("store i8* "+sl+", i8** "+c1);
    return closure;
  } else if (e->isBinop()) {
    auto z = e->toBinop();
    auto tp = llvm_type(z->left);
    string llvm_op = (tp == "i32")
         ? (z->op == "PLUS") ? "add"
         : (z->op == "TIMES") ? "mul"
         : (z->op == "MINUS") ? "sub"
         : (z->op == "DIV") ? "div"
         : (z->op == "MOD") ? "srem"
         : (z->op == "EQ") ? "icmp eq"
         : (z->op == "NEQ") ? "icmp ne"
         : (z->op == "GT") ? "icmp sgt"
         : (z->op == "GEQ") ? "icmp sge"
         : (z->op == "LT") ? "icmp slt"
         : (z->op == "LEQ") ? "icmp sle"
         : splc_error("Wrong binary int operation "+e->toString())
       : (tp == "float")
         ? (z->op == "PLUS") ? "fadd"
         : (z->op == "TIMES") ? "fmul"
         : (z->op == "MINUS") ? "fsub"
         : (z->op == "DIV") ? "fdiv"
         : (z->op == "EQ") ? "fcmp oeq"
         : (z->op == "NEQ") ? "fcmp one"
         : (z->op == "GT") ? "fcmp ogt"
         : (z->op == "GEQ") ? "fcmp oge"
         : (z->op == "LT") ? "fcmp olt"
         : (z->op == "LEQ") ? "fcmp ole"
         : splc_error("Wrong binary float operation "+e->toString())
       : (tp == "i1")
         ? (z->op == "EQ") ? "icmp eq"
         : (z->op == "NEQ") ? "icmp ne"
         : (z->op == "AND") ? "and"
         : (z->op == "OR") ? "or"
         : splc_error("Wrong boolean operation "+e->toString())
       : (z->op == "EQ") ? "icmp eq"
         : (z->op == "NEQ") ? "icmp ne"
         : splc_error("Wrong binary operation "+e->toString()+" on type "+tp);
    return llvm(llvm_op+" "+tp+" "+emit(z->left)+", "+emit(z->right));
  } else if (e->isUnop()) {
    auto z = e->toUnop();
    if (z->op == "NOT")
      return llvm("xor i1 "+emit(z->operand)+", 1");
    else if (z->op == "MINUS") {
      auto tp = llvm_type(z->operand);
      if (tp == "i32")
        return llvm("sub i32 0, "+emit(z->operand));
      else return llvm("fsub float 0.0, "+emit(z->operand));
    } else splc_error("Wrong unary operation "+e->toString());
  } else if (e->isCall()) {
    auto z = e->toCall();
    if (z->address->isAddress()) {
      auto ltp = llvm_type(e);
      auto sl = z->static_link;
      auto as = llvm_type(sl)+" "+emit(sl);
      for ( auto a: z->arguments )
        as += ", "+llvm_type(a)+" "+emit(a);
      return llvm("call "+ltp+" @"+z->address->toAddress()->value
                  +" ( "+as+" )");
    } else {
      // call function is the closure of an anonymous function
      auto tp = type_of(z->address);
      auto closure = emit(z->address);
      auto ctp = tp->toPointerType()->address;
      auto lctp = ltype(ctp);
      auto lf = ctp->toTupleType()->components[0];
      auto otp = lf->toPointerType()->address->toFunctionType()->result_type;
      auto frame = ctp->toTupleType()->components[1];
      auto fa = llvm("getelementptr "+lctp+", "+lctp+"* "+closure
                     +", i32 0, i32 0");
      auto ff = llvm("load "+ltype(lf)+", "+ltype(lf)+"* "+fa
                     +"   ; get the function address from closure");
      auto sl = llvm("getelementptr "+lctp+", "+lctp+"* "+closure
                     +", i32 0, i32 1");
      auto fn = ltype(frame);
      auto ss = llvm("load "+fn+", "+fn+"* "
                     +sl+"   ; get the static link from closure");
      auto as = fn+" "+ss;
      for ( auto a: z->arguments ) {
        as += ", "+llvm_type(a)+" "+emit(a);
      }
      return llvm("call "+ltype(otp)+" "+ff+" ( "+as+" )");
    }
  } else if (e->isAllocate()) {
    auto z = e->toAllocate();
    auto sz = llvm("zext i32 "+emit(z->size)+" to i64");
    auto p = llvm("call noalias i8* @"+malloc_op+"(i64 "+sz+")");
    return llvm("bitcast i8* "+p+" to "+ltype(z->otype)+"*");
  } else if (e->isTypeSize()) {
    auto ltp = ltype(e->toTypeSize()->object_type);
    if (ltp == "i8") return "1";
    else if (ltp == "i32") return "4";
    else if (ltp == "i64") return "8";
    else if (ltp == "float") return "4";
    auto size = llvm("getelementptr "+ltp+", "+ltp
                     +"* null, i32 1 ; calculate the size in bytes");
    return llvm("ptrtoint "+ltp+"* "+size+" to i32");
  } else if (e->isESeq()) {
    auto z = e->toESeq();
    emit(z->stmt,mkNoType());
    return emit(z->value);
  } else if (e->isCoerce()) {
    auto z = e->toCoerce();
    auto cu = emit(z->value);
    if (*z->from_type == *z->to_type)
      return cu;
    else if (z->from_type->isIntType() && z->to_type->isFloatType())
      return llvm("sitofp i32 "+cu+" to float");
    else return llvm("bitcast "+ltype(z->from_type)+" "
                     +cu+" to "+ltype(z->to_type));
  } else splc_error("Wrong IRexp: "+e->toString());
}

/** true, if the type tp contains a functional type */
bool contains_functional ( Type tp ) {
  if (tp->isFunctionType())
    return true;
  else if (tp->isArrayType())
    return contains_functional(tp->toArrayType()->element_type);
  else if (tp->isTupleType()) {
    for ( auto x: tp->toTupleType()->components )
      if (contains_functional(x))
        return true;
    return false;
  } else if (tp->isPointerType())
    return contains_functional(tp->toPointerType()->address);
  else return false;
}

/** generate LLVM code from an IR statement */
void emit ( IRstmt e, Type return_type ) {
  out << e->pretty("; ") << endl;
  after_jump = false;
  if (e->isMove()) {
    auto z = e->toMove();
    auto d = z->destination->toMem()->address;
    auto dtp = type_of(z->destination);
    auto stp = type_of(z->source);
    auto source = emit(mkCoerce(z->source,stp,dtp));
    llvms("store "+ltype(dtp)+" "+source+", "+ltype(dtp)+"* "+emit(d));
  } else if (e->isLabel())
    llvm_label(e->toLabel()->name);
  else if (e->isJump()) {
    llvms("br label %"+e->toJump()->label);
    after_jump = true;
  } else if (e->isCJump()) {
    auto z = e->toCJump();
    auto next = new_label();
    llvms("br i1 "+emit(z->condition)+", label %"+z->label+", label %"+next);
    after_jump = true;
    llvm_label(next);
  } else if (e->isCallP()) {
    auto z = e->toCallP();
    if (z->address->isAddress()) {
      auto sl = z->static_link;
      auto as = llvm_type(sl)+" "+emit(sl);
      for ( auto a: z->arguments )
        as += ", "+llvm_type(a)+" "+emit(a);
      llvms("call void @"+z->address->toAddress()->value+" ( "+as+" )");
    } else {
      // the call function is the closure of an anonymous function
      auto tp = type_of(z->address);
      auto closure = emit(z->address);
      auto ctp = tp->toPointerType()->address;
      auto lctp = ltype(ctp);
      auto lf = ctp->toTupleType()->components[0];
      auto frame = ctp->toTupleType()->components[1];
      auto fa = llvm("getelementptr "+lctp+", "+lctp+"* "+closure
                     +", i32 0, i32 0");
      auto ff = llvm("load "+ltype(lf)+", "+ltype(lf)+"* "+fa
                     +"   ; get the function address from closure");
      auto sl = llvm("getelementptr "+lctp+", "+lctp+"* "+closure
                     +", i32 0, i32 1");
      auto fn = ltype(frame);
      auto ss = llvm("load "+fn+", "+fn+"* "+sl+"   ; get the static link from closure");
      auto as = fn+" "+ss;
      for ( auto a: z->arguments )
        as += ", "+llvm_type(a)+" "+emit(a);
      llvms("call void "+ff+" ( "+as+" )");
    }
  } else if (e->isAssert()) {
    auto z = e->toAssert();
    auto cont = new_label();
    auto next = new_label();
    llvms("br i1 "+emit(z->condition)+", label %"+cont+", label %"+next);
    after_jump = true;
    llvm_label(next);
    emit(mkSystemCall("WRITE_STRING",mkStringValue("Assertion error: "
                                         +z->condition->toString())),
         return_type);
    llvms("call void @exit ( i32 1 )");
    llvms("unreachable");
    llvm_label(cont);
  } else if (e->isReturn()) {
    auto z = e->toReturn();
    if (z->value->isVoidValue())
      llvms("ret void");
    else {
      auto etp = ltype(return_type);
      auto vtp = llvm_type(z->value);
      auto sv = (etp != vtp) ? llvm("bitcast "+vtp+" "+emit(z->value)+" to "+etp)
                             : emit(z->value);
      llvms("ret "+etp+" "+sv);
    }
  } else if (e->isSeq()) {
    for ( auto s: e->toSeq()->stmts )
      emit(s,return_type);
  } else if (e->isSystemCall()) {
    auto z = e->toSystemCall();
    if (z->name == "WRITE_INT")
      llvms("call i32 @puti ( i32 "+emit(z->arg)+" )");
    else if (z->name == "WRITE_FLOAT")
      llvms("call i32 @putf ( float "+emit(z->arg)+" )");
    else if (z->name == "WRITE_STRING")
      if (z->arg->isStringValue() && z->arg->toStringValue()->value == "\\n") {
        auto nl = llvm("getelementptr inbounds [2 x i8], [2 x i8]* @.new_line, i32 0, i32 0");
        llvms("call i32(i8*, ...) @printf ( i8* "+nl+" )");
      } else {
        auto v = llvm("getelementptr inbounds {i32,i8*}, {i32,i8*}* "+emit(z->arg)
                      +", i32 0, i32 1");
        auto w = llvm("load i8*, i8** "+v);
        llvms("call i32(i8*, ...) @printf ( i8* "+w+" )");
      }
    else if (z->name == "READ_INT")
      llvms("call i32 @geti ( i32* "+emit(z->arg)+" )");
    else if (z->name == "READ_FLOAT")
      llvms("call i32 @getf ( float* "+emit(z->arg)+" )");
    else if (z->name == "READ_STRING")
      llvms("call i32 @getf ( i8* "+emit(z->arg)+" )");
  } else splc_error("Wrong IRstmt: "+e->toString());
}

/** emit LLVM code for a declaration */
void emit ( IRdecl e ) {
  if (e->isIRFuncDecl()) {
    auto z = e->toIRFuncDecl();
    if (z->label == "main") {
      frame_type = mkTupleType(z->frame);
      frame_type_name = "main_frame";
      register_count = 0;
      out << "define i32 @main ( i32 %argc, i8** %argv ) {\n";
      llvms("%fp = alloca %main_frame, align 16");
      for ( auto x: z->body )
        emit(x,mkNoType());
      llvms("ret i32 0");
      out << "}\n\n";
    } else {
      frame_type_name = z->label+"_frame";
      frame_type = mkTupleType(z->frame);
      register_count = 0;
      auto sl_type = ltype(z->frame[0]);
      auto args = sl_type;
      for ( auto a: z->formal_params )
        args += ", "+ltype(a);
      if (z->result_type->isNoType())
        out << "define void @" << z->label << " ( " << args << " ) {\n";
      else out << "define " << ltype(z->result_type) << " @" << z->label
               << " ( " << args << " ) {\n";
      if (!contains_functional(z->result_type))
        llvms("%fp = alloca %"+frame_type_name+", align 16");
      else {
        // the function frame that returns a closure must be allocated in the heap
        auto size = emit(mkTypeSize(frame_type));   // emit code to calculate frame size
        auto sz = llvm("zext i32 "+size+" to i64");
        auto p = llvm("call noalias i8* @"+malloc_op
                      +"(i64 "+sz+")   ; allocate the frame in the heap");
        llvms("%fp = bitcast i8* "+p+" to %"+frame_type_name+"*");
      }
      auto sl = llvm("getelementptr %"+frame_type_name+", %"+frame_type_name
                     +"* %fp, i32 0, i32 0");
      llvms("store "+sl_type+" %0, "+sl_type+"* "+sl);
      int i = 0;
      for ( auto x: z->formal_params ) {
        auto tp = ltype(x);
        auto a = llvm("getelementptr %"+frame_type_name+", %"+frame_type_name
                      +"* %fp, i32 0, i32 "+to_string(++i));
        llvms("store "+tp+" %"+to_string(i)+", "+tp+"* "+a+"   ; store the "
              +to_string(i)+"'th argument in the frame");
      }
      for ( auto x: z->body )
        emit(x,z->result_type);
      if (z->result_type->isNoType())
        llvms("ret void");
      else llvms("unreachable");
      out << "}\n\n";
    }
  } else if (e->isIRTypeDecl()) {
    auto z = e->toIRTypeDecl();
    auto ltp = ltype(expand_named_type(z->hasType));
    out << "%" << z->label << " = type " << ltp << "\n\n";
  }
}

/** emit LLVM code for the program */
void emit ( list<tuple<string,IRdecl>*>* decls, const string &llvm_file ) {
  out.open(llvm_file.c_str());
  frame_type_name = "main_frame";
  ir_declarations = decls;
  for ( auto d: *decls )
    if (get<1>(*d)->isIRTypeDecl())
      emit(get<1>(*d));
  for ( auto d: *decls )
    if (get<1>(*d)->isIRFuncDecl())
      emit(get<1>(*d));
  auto quote { "\"" };
  int i = 0;
  for ( auto s: globals ) {
     auto len = s.length()+1;
     out << "@S_c_" << i << " = private unnamed_addr constant [" << len
         << " x i8] c\"" << s << "\\00\", align 1\n";
     out << "@S_" << i << " = private unnamed_addr constant {i32,i8*} { i32 "
         << len << ", i8* bitcast([" << len << " x i8]* @S_c_" << i << " to i8*) }\n";
     i++;
  }
  out << "\ndeclare noalias i8* @"+malloc_op+" ( i64 )\n\n";
  ifstream in;
  string line;
  // append the run-time library (unix path)
  in.open(spl_home+"/include/spl_llvm.h");
  while (getline(in,line))
    out << line << endl;
  in.close();
  out.close();
}
