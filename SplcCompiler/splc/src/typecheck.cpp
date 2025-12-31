//Mohamad Nabih Alkhateeb
//Student ID: 1002170786 
//CSE 4305: Project 5 - Type Checking
//typecheck.cpp
//10/28/2025

/*  
   __ __       _    _ _       _    _  _   
  |  \  \ ___ | |_ | \ | ___ | |_ <_>| |_ 
  |     |/ . \| . ||   |<_> || . \| || . |
  |_|_|_|\___/|_|_||_\_|<___||___/|_||_|_|

 */ 

 /* 
   SPL Typechecker (Project 5)

      - This file handles the typechecking part of the SPL compiler. It makes sure
     all statements and expressions follow the language’s type rules and that
     things like variables, functions, arrays, tuples, and records are used
     correctly.

   - One of the tricky parts was getting the typechecking order and trace output
     to match the solution exactly. I had to adjust how scopes and expressions
     were checked, especially with nested arrays and records, to keep everything
     consistent.


   - Another issue was the type environment behavior across scopes (especially 
     with function definitions). To solve this, i paid extra attention to 
     correctly pushing and popping scopes during block and function evaluation. 

   - The Makefile has been updated to use the solution parser for consistency 
     when testing against the provided test cases even though my own parser was working just fine.

   - I used the following command to redirect the results into a txt file for easier
     inspection and comparison with the expected output ( since the output is very large)

         cat tests/project5.txt | bin/solution -project 5 > output.txt
         cat tests/project5.txt | bin/splc -project 5 > output.txt

    - Finally, I tried to match the exptected output of the solution to the best I could,
     Although I do not beleive that the solution is necessarily 100% correct.   

  
*/



#include <iostream>
#include <list>
#include "Node.h"
#include "AST.h"

extern bool trace_typecheck;
extern int tabs;

/** symbol table to store declarations */
Environment<Declaration>* st = new (GC) Environment<Declaration>();

Type int_type   = mkIntType();
Type float_type = mkFloatType();
Type bool_type  = mkBooleanType();
Type string_type= mkStringType();

int clear_tabs () {
  tabs = 0;
  return tabs;
}

#define type_error(e,msg) throw runtime_error("Typechecking error on line "+to_string(e->line_number)+" at "+to_string(e->position+clear_tabs())+": "+msg)

/** Print the type into a string (used for printing the result) */
string toString ( Type v ) {
  if (v->isIntType()) return "int";
  else if (v->isFloatType()) return "float";
  else if (v->isStringType()) return "string";
  else if (v->isBooleanType()) return "boolean";
  else if (v->isArrayType())
    return "array["+toString(v->toArrayType()->element_type)+"]";
  else if (v->isTupleType()) {
    auto z = v->toTupleType();
    string s = "( ";
    int i = 0;
    for ( auto t: z->components ) {
      if (i > 0) s += ", ";
      s += toString(t);
      i++;
    }
    return s+" )";
  } else if (v->isRecordType()) {
    auto z = v->toRecordType();
    string s = "{ ";
    int i = 0;
    for ( auto t: z->components ) {
      if (i > 0) s += ", ";
      s += get<0>(t)+": "+toString(get<1>(t));
      i++;
    }
    return s+" }";
  } else if (v->isFunctionType()) {
    auto z = v->toFunctionType();
    string s = "( ";
    int i = 0;
    for ( auto t: z->formal_params ) {
      if (i > 0) s += ", ";
      s += toString(t);
      i++;
    }
    return s+" ) -> "+toString(z->result_type);
  } else return "()";
}

Type expand_type ( Type tp );

/** Check if tp is a valid type */
void check_type ( Type tp ) {
  if (tp->isIntType() || tp->isByteType() || tp->isFloatType() || tp->isStringType()
      || tp->isBooleanType() || tp->isAnyType() || tp->isNoType())
    return;
  else if (tp->isNamedType()) {
    auto ntp = tp->toNamedType();
    if (!st->exists(ntp->name))
      type_error(tp,"Undeclared type: "+ntp->name);
    check_type(expand_type(tp));
  } else if (tp->isArrayType())
    check_type(tp->toArrayType()->element_type);
  else if (tp->isRecordType()) {
    auto rt = tp->toRecordType();
    // Reject duplicate attribute names in record *types*
    for (size_t i = 0; i < rt->components.size(); ++i)
      for (size_t j = i + 1; j < rt->components.size(); ++j)
        if (get<0>(rt->components[i]) == get<0>(rt->components[j]))
          type_error(tp,"Duplicate attribute name: "+get<0>(rt->components[i]));
    for ( auto c: rt->components )
      check_type(get<1>(c));
  } else if (tp->isTupleType()) {
    for ( auto e: tp->toTupleType()->components )
      check_type(e);
  } else if (tp->isFunctionType()) {
    auto ft = tp->toFunctionType();
    for ( auto p: ft->formal_params )
      check_type(p);
    check_type(ft->result_type);
  } else if (tp->isPointerType())
    check_type(tp->toPointerType()->address);
}

/** If tp is a named type, expand it */
Type expand_type ( Type tp ) {
  if (tp->isNamedType()) {
    auto ntp = tp->toNamedType();
    if (st->exists(ntp->name)) {
      auto td = st->lookup(ntp->name);
      if (td->isTypeDeclaration())
        return expand_type(td->toTypeDeclaration()->hastype);
    }
    type_error(tp,"Undeclared type: "+ntp->name);
  } else return tp;
  
}

/** returns true if the types tp1 and tp2 are equal under structural equivalence */
bool typeEquivalence ( Type tp1, Type tp2 ) {
  if (*tp1 == *tp2 || tp1->isAnyType() || tp2->isAnyType())
    return true;

  Type etp1 = expand_type(tp1);
  Type etp2 = expand_type(tp2);

  if (etp1->isIntType()     && etp2->isIntType())     return true;
  if (etp1->isFloatType()   && etp2->isFloatType())   return true;
  if (etp1->isStringType()  && etp2->isStringType())  return true;
  if (etp1->isBooleanType() && etp2->isBooleanType()) return true;
  if (etp1->isByteType()    && etp2->isByteType())    return true;
  if (etp1->isNoType()      && etp2->isNoType())      return true;

  if (etp1->isArrayType() && etp2->isArrayType())
    return typeEquivalence(etp1->toArrayType()->element_type,
                           etp2->toArrayType()->element_type);

  if (etp1->isRecordType() && etp2->isRecordType()) {
    auto r1 = etp1->toRecordType(), r2 = etp2->toRecordType();
    if (r1->components.size() != r2->components.size()) return false;
    for (size_t i = 0; i < r1->components.size(); ++i)
      if (get<0>(r1->components[i]) != get<0>(r2->components[i]) ||
          !typeEquivalence(get<1>(r1->components[i]), get<1>(r2->components[i])))
        return false;
    return true;
  }

  if (etp1->isTupleType() && etp2->isTupleType()) {
    auto t1 = etp1->toTupleType(), t2 = etp2->toTupleType();
    if (t1->components.size() != t2->components.size()) return false;
    for (size_t i = 0; i < t1->components.size(); ++i)
      if (!typeEquivalence(t1->components[i], t2->components[i])) return false;
    return true;
  }

  if (etp1->isFunctionType() && etp2->isFunctionType()) {
    auto f1 = etp1->toFunctionType(), f2 = etp2->toFunctionType();
    if (f1->formal_params.size() != f2->formal_params.size()) return false;
    if (!typeEquivalence(f1->result_type, f2->result_type)) return false;
    for (size_t i = 0; i < f1->formal_params.size(); ++i)
      if (!typeEquivalence(f1->formal_params[i], f2->formal_params[i])) return false;
    return true;
  }

  if (etp1->isPointerType() && etp2->isPointerType())
    return typeEquivalence(etp1->toPointerType()->address,
                           etp2->toPointerType()->address);

  return false;
}

void typecheck ( Stmt e, Type expected_type );

Type typecheck_ ( Expr e );
Type typecheck ( Expr e ) {
  if (trace_typecheck)
    cout << string((tabs++)*3,' ') << "** " << e->toString() << endl;
  auto tp = (e->tpe != nullptr) ? e->tpe : typecheck_(e);
  e->tpe = tp;   // store the type in the Expr node
  if (trace_typecheck)
    cout << string((--tabs)*3,' ') << "-> " << tp->toString() << endl;
  return tp;
}

/** typecheck an expression AST */
Type typecheck_ ( Expr e ) {
  if (e->isIntConst())
    return int_type;
  else if (e->isFloatConst())
    return float_type;
  else if (e->isStringConst())
    return string_type;
  else if (e->isBooleanConst())
    return bool_type;
  else if (e->isVar()) {
    auto z = e->toVar();
    if (!st->exists(z->name))
      type_error(e,"Undefined variable: "+z->name);
    auto d = st->lookup(z->name);
    if (d->isVarDeclaration())
      return d->toVarDeclaration()->vartype;
    else type_error(e,z->name+" is not a variable");
  } else if (e->isNullExp())
    return mkAnyType();
  else if (e->isBinOpExp()) {
    auto z = e->toBinOpExp();
    Type ltp = typecheck(z->left);
    Type rtp = typecheck(z->right);
    if (z->op=="plus" || z->op=="minus" || z->op=="times") {
      if (ltp->isIntType() && rtp->isIntType()) return int_type;
      if ((ltp->isIntType() || ltp->isFloatType()) && (rtp->isIntType() || rtp->isFloatType())) return float_type;
      type_error(e,"Binary arithmetic operations can only be applied to integer or real numbers: "+e->toString());
    } else if (z->op=="div") {
      if ((ltp->isIntType() || ltp->isFloatType()) && (rtp->isIntType() || rtp->isFloatType())) return float_type;
      type_error(e,"Division must be applied to numeric types");
    } else if (z->op=="mod") {
      if (ltp->isIntType() && rtp->isIntType()) return int_type;
      type_error(e,"Modulo must be applied to integers");
    } else if (z->op=="gt" || z->op=="lt" || z->op=="geq" || z->op=="leq") {
      if ((ltp->isIntType() || ltp->isFloatType()) && (rtp->isIntType() || rtp->isFloatType())) return bool_type;
      type_error(e,"Relational operation must be applied to numeric types");
    } else if (z->op=="eq" || z->op=="neq") {
      if (typeEquivalence(ltp,rtp)) return bool_type;
      type_error(e,"Incompatible types in equality: "+e->toString());
    } else if (z->op=="and" || z->op=="or") {
      if (ltp->isBooleanType() && rtp->isBooleanType()) return bool_type;
      type_error(e,"Boolean operation must be applied to booleans");
    } else type_error(e,"Unknown binary operator");
  } else if (e->isUnOpExp()) {
    auto z = e->toUnOpExp();
    Type tp = typecheck(z->operand);
    if (z->op=="not") {
      if (tp->isBooleanType()) return bool_type;
      type_error(e,"Not must be applied to boolean");
    } else if (z->op=="minus") {
      if (tp->isIntType() || tp->isFloatType()) return tp;
      type_error(e,"Minus must be applied to numeric type");
    } else type_error(e,"Unknown unary operator");
  } else if (e->isArrayDeref()) {
    auto z = e->toArrayDeref();

    // Index is checked first so the trace matches the expected order

    Type itp = typecheck(z->index);
    if (!itp->isIntType())
      type_error(e,"The array index must be an integer: "+e->toString());
    Type atp = typecheck(z->array);
    if (atp->isArrayType())
      return atp->toArrayType()->element_type;
    type_error(e,"Array dereference must be applied to array");
  } else if (e->isRecordDeref()) {
    auto z = e->toRecordDeref();
    Type rtp = typecheck(z->record);
    if (z->attribute=="length" && rtp->isArrayType())
      return int_type;
    rtp = expand_type(rtp);
    if (rtp->isRecordType()) {
      for ( auto comp: rtp->toRecordType()->components )
        if (get<0>(comp) == z->attribute)
          return get<1>(comp);
      type_error(e,"Record does not have the attribute: "+z->attribute);
    }
    type_error(e,"Record dereference must be applied to record");
  } else if (e->isTupleDeref()) {
    auto z = e->toTupleDeref();
    Type ttp = expand_type(typecheck(z->tuple));
    if (ttp->isTupleType()) {
      auto tt = ttp->toTupleType();
      if (z->index < 0 || z->index >= (int)tt->components.size())
        type_error(e,"Tuple index "+to_string(z->index)+" is invalid");
      return tt->components[z->index];
    }
    type_error(e,"Tuple projection can only be done on tuples: "+e->toString());
  } else if (e->isRecordExp()) {
    auto z = e->toRecordExp();

    // Type each value first (so its trace appears), then detect duplicates

    vector<tuple<string,Type>> comps;
    vector<string> names_seen;
    for ( auto &c: z->components ) {
      const string &name = get<0>(c);
      Type val_tp = typecheck(get<1>(c));
      for (const auto &s : names_seen)
        if (s == name) type_error(e,"Duplicate record attribute: "+name);
      comps.emplace_back(name,val_tp);
      names_seen.push_back(name);
    }
    return mkRecordType(comps);
  } else if (e->isArrayExp()) {
    auto z = e->toArrayExp();
    if (z->elements.empty())
      type_error(e,"Array must have at least one element");
    Type etp = typecheck(z->elements[0]);
    for ( auto el: z->elements ) {
      Type t = typecheck(el);
      if (!typeEquivalence(etp,t)) {
        string desc = el->isVar()? "Var(\""+el->toVar()->name+"\")" : el->toString();
        type_error(e,"The type of "+desc+" in the array is "+toString(t)+", but it was expected to be "+toString(etp));
      }
    }
    return mkArrayType(etp);
  } else if (e->isArrayGen()) {
    auto z = e->toArrayGen();
    Type ltp = typecheck(z->length);
    Type vtp = typecheck(z->value);
    if (!ltp->isIntType())
      type_error(e,"Array length must be integer");
    return mkArrayType(vtp);
  } else if (e->isTupleExp()) {
    auto z = e->toTupleExp();
    vector<Type> comps;
    for ( auto el: z->elements )
      comps.push_back(typecheck(el));
    return mkTupleType(comps);
  } else if (e->isFunctionExp()) {
    auto z = e->toFunctionExp();

    // Validate declared types and typecheck the body to emit the BlockSt trace inline
    vector<Type> param_types;
    for ( auto &p: z->formal_params ) {
      check_type(get<1>(p));
      param_types.push_back(get<1>(p));
    }
    check_type(z->result_type);
    st->begin_scope();
    for ( auto &p: z->formal_params )
      st->insert(get<0>(p), mkVarDeclaration(get<1>(p), 0, 0));
    typecheck(z->body, z->result_type);
    st->end_scope();
    return mkFunctionType(param_types, z->result_type);
  } else if (e->isCallExp()) {
    auto z = e->toCallExp();
    if (!st->exists(z->name))
      type_error(e,"Undefined function: "+z->name);
    auto d = st->lookup(z->name);
    if (d->isFuncDeclaration()) {
      auto fd = d->toFuncDeclaration();
      if (z->arguments.size() != fd->formal_params.size())
        type_error(e,"Number of formal parameters does not match the number of arguments in call: "
                   +e->toString());
      int i = 0;
      for ( auto arg: z->arguments )
        if (!typeEquivalence(typecheck(arg),get<1>(fd->formal_params[i++])))
          type_error(e,"The type of call argument "+arg->toString()
                     +" does not match the type of the formal parameter: "
                     +toString(get<1>(fd->formal_params[i-1])));
      return fd->result_type;
    } else if (d->isVarDeclaration() && d->toVarDeclaration()->vartype->isFunctionType()) {
      auto ft = d->toVarDeclaration()->vartype->toFunctionType();
      if (z->arguments.size() != ft->formal_params.size())
        type_error(e,"Number of formal parameters does not match the number of arguments in call: "
                   +e->toString());
      int i = 0;
      for ( auto arg: z->arguments )
        if (!typeEquivalence(typecheck(arg),ft->formal_params[i++]))
          type_error(e,"The type of call argument "+arg->toString()
                     +" does not match the type of the formal parameter: "
                     +toString(ft->formal_params[i-1]));
      return ft->result_type;
    } else type_error(e,"Not a function: "+z->name);
  }

  else type_error(e,"Unknown expression: "+e->toString());
}

void typecheck_ ( Stmt e, Type expected_type );
void typecheck ( Stmt e, Type expected_type ) {
  if (trace_typecheck)
    cout << string((tabs++)*3,' ') << "** " << e->toString() << endl;
  typecheck_(e,expected_type);
  if (trace_typecheck)
    cout << string((--tabs)*3,' ') << "->" << endl;
}

/** Typecheck a statement using the expected type of the return value 
    from the current function */
void typecheck_ ( Stmt e, Type expected_type ) {
  if (e->isAssignSt()) {
    auto z = e->toAssignSt();
    // Evaluate source first so the trace order matches the expected output
    Type src_type  = typecheck(z->source);
    Type dest_type = typecheck(z->destination);
    if (!typeEquivalence(dest_type,src_type))
      type_error(e,"Incompatible types in assignment: "+e->toString());
  }

  else if (e->isCallSt()) {
    auto z = e->toCallSt();
    if (!st->exists(z->name))
      type_error(e,"Undefined function: "+z->name);
    auto d = st->lookup(z->name);
    if (d->isFuncDeclaration()) {
      auto fd = d->toFuncDeclaration();
      if (z->arguments.size() != fd->formal_params.size())
        type_error(e,"Number of formal parameters does not match the number of arguments in call: "
                   +e->toString());
      int i = 0;
      for ( auto a: z->arguments )
        if (!typeEquivalence(typecheck(a),get<1>(fd->formal_params[i++])))
          type_error(e,"The type of call argument "+a->toString()
                     +" does not match the type of the formal parameter: "
                     +toString(get<1>(fd->formal_params[i-1])));
    } else type_error(e,"Not a function: "+z->name);
  }

  else if (e->isReadSt()) {
    auto z = e->toReadSt();
    for ( auto a: z->arguments ) {
      Type t = typecheck(a);
      if (!t->isIntType() && !t->isFloatType() && !t->isStringType())
        type_error(e,"Read can only be applied to numeric or string variables");
    }
  }

  else if (e->isPrintSt()) {
    for ( auto a: e->toPrintSt()->arguments )
      typecheck(a);
  }

  else if (e->isIfSt()) {
    auto z = e->toIfSt();
    Type ct = typecheck(z->condition);
    if (!ct->isBooleanType())
      type_error(e,"Expected a boolean in IF test: "+z->condition->toString());
    typecheck(z->then_stmt,expected_type);
    typecheck(z->else_stmt,expected_type);
  }

  else if (e->isWhileSt()) {
    auto z = e->toWhileSt();
    Type ct = typecheck(z->condition);
    if (!ct->isBooleanType())
      type_error(e,"Expected boolean in WHILE test: "+z->condition->toString());
    typecheck(z->body,expected_type);
  }

  else if (e->isLoopSt()) {
    typecheck(e->toLoopSt()->body,expected_type);
  }

  else if (e->isForSt()) {
    auto z = e->toForSt();
    Type it  = typecheck(z->initial);
    Type lt  = typecheck(z->last);
    Type inc = typecheck(z->increment);
    if (!it->isIntType() || !lt->isIntType() || !inc->isIntType())
      type_error(e,"FOR loop parameters must be integers");
    st->begin_scope();
    st->insert(z->variable, mkVarDeclaration(int_type, 0, 0));
    typecheck(z->body,expected_type);
    st->end_scope();
  }

  else if (e->isExitSt()) {
  }

  else if (e->isReturnValueSt()) {
    Type rt = typecheck(e->toReturnValueSt()->value);
    if (!typeEquivalence(rt,expected_type))
      type_error(e,"Return type does not match expected type");
  }

  else if (e->isReturnSt()) {
    if (!expected_type->isNoType())
      type_error(e,"Function must return a value");
  }

  else if (e->isBlockSt()) {
    auto z = e->toBlockSt();
    st->begin_scope();
    for ( auto s: z->stmts )
      typecheck(s,expected_type);
    st->end_scope();
  }

  else if (e->isTypeDef()) {
    auto z = e->toTypeDef();
    check_type(z->hasType);
    st->insert(z->name, mkTypeDeclaration(z->hasType));  // allow shadowing
  }

  else if (e->isVarDef()) {
    auto z = e->toVarDef();

    // AnyType adopts the initializer's type (if non-null). Otherwise we enforce the declared type.
    Type declared = z->hasType;
    bool has_val = (z->value != nullptr);
    if (!declared->isAnyType())
      check_type(declared);

    Type init_t = nullptr;
    bool is_null = false;
    if (has_val) {
      init_t = typecheck(z->value);
      is_null = z->value->isNullExp();
    }

    if (declared->isAnyType()) {
      if (has_val && !is_null)
        declared = init_t;
    } else {
      if (has_val && !is_null && !typeEquivalence(declared,init_t))
        type_error(e,"Incompatible types in variable definition");
    }

    st->insert(z->name, mkVarDeclaration(declared, 0, 0)); // allow shadowing
  }

  else if (e->isFuncDef()) {
    auto z = e->toFuncDef();
    check_type(z->result_type);
    for ( auto p: z->formal_params )
      check_type(get<1>(p));
    st->insert(z->name, mkFuncDeclaration(z->formal_params, z->result_type, "", 0, 0));
    st->begin_scope();
    for ( auto p: z->formal_params )
      st->insert(get<0>(p), mkVarDeclaration(get<1>(p), 0, 0));
    typecheck(z->body, z->result_type);
    st->end_scope();
  }

  else type_error(e,"Unrecognized statement: "+e->toString());
}

/** typecheck the main program */
void typecheck ( Program e ) {
  typecheck(e->body,mkNoType());
}

// End of work: typecheck.cpp - 10/28/2025 - Nabih