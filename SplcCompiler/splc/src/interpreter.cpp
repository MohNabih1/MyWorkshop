//Mohamad Nabih Alkhateeb
//Student ID: 1002170786 
//CSE 4305: Project 6 - Interpreter
//interpreter.cpp
//11/18/2025

/*  
   __ __       _    _ _       _    _  _   
  |  \  \ ___ | |_ | \ | ___ | |_ <_>| |_ 
  |     |/ . \| . ||   |<_> || . \| || . |
  |_|_|_|\___/|_|_||_\_|<___||___/|_||_|_|

*/ 

/* 
   SPL Interpreter (Project 6)

   *** IMPORTANT: THIS PROJECT MUST BE COMPILED AND RUN ON UBUNTU WSL 22.04 ***
   *** RUNNING ON OTHER ENVIRONMENTS (like ubuntu 24.04) MIGHT RESULT IN A SEGMENTATION FAULT: (CORE DUMPED)***
   *** PLEASE MAKE SURE TO RUN THE MAKE COMMAND ON WSL 22.04 TO AVOID SEGMENTATION FAULTS***

   - this file implements the interpreter for the SPL language. it walks the
     AST and executes statements and expressions according to the language
     semantics.

   - I decided to use the solution's typechecker to ensure consistency
     with the solution's output.


   - I also had to be careful with how closures capture their environment and
     how return statements are modeled using exceptions, so that recursion and
     local variables behave exactly like in the solutoin's output.


   - To debug and compare results, I redirected the interpreter trace for the
     project 6 test file into a text file and diffed it against the solution:

         cat tests/project6.txt | bin/splc -project 6 > outputsplc.txt

   - overall, i tried to match the solution's interpreter's behavior as closely
     as possible although I do not beleive that the solution is necessarily 100% correct.
*/



#include <iostream>
#include <sstream>
#include <list>
#include "Node.h"
#include "AST.h"

extern bool trace_eval;
extern int tabs;

typedef Environment<Value>* Env;

Env global_env = new (GC) Environment<Value>();

Value true_val = mkBooleanVal(true);
Value false_val = mkBooleanVal(false);

bool loop_exit = false;

/** Coerce a value to a float */
float toFloat ( Value v ) {
  if (v->isFloatVal())
    return v->toFloatVal()->value;
  else if (v->isIntVal())
    return (float)v->toIntVal()->value;
  else splc_error("Cannot coerce "+v->pretty("")+" to a float");
}

/** Print the value into a string */
string toString ( Value v ) {
  if (v->isIntVal())
    return to_string(v->toIntVal()->value);
  else if (v->isFloatVal()) {
    ostringstream out;
    out << v->toFloatVal()->value;
    return out.str();
  } else if (v->isBooleanVal())
    return (v->toBooleanVal()->value) ? "true" : "false";
  else if (v->isStringVal())
    return "\""+v->toStringVal()->value+"\"";
  else if (v->isVoidVal())
    return "()";
  else if (v->isRecordVal()) {
    string s = "{ ";
    int i = 0;
    for ( auto t: v->toRecordVal()->value ) {
      if (i > 0) s += ", ";
      s += get<0>(t)+" = "+toString(get<1>(t));
      i++;
    }
    return s+" }";
  } else if (v->isVectorVal()) {
    string s = "( ";
    int i = 0;
    for ( auto x: v->toVectorVal()->value ) {
      if (i > 0) s += ", ";
      s += toString(x);
      i++;
    }
    return s+" )";
  } else if (v->isClosureVal())
    return "<closure>";
  else splc_error("Expected a value to print: " + v->pretty());
}

void eval ( Stmt e, Env env );

Value eval_ ( Expr e, Env env );
Value eval ( Expr e, Env env ) {
  if (trace_eval)
    cout << string((tabs++)*3,' ') << "** " << e->toString() << endl;
  auto res = eval_(e,env);
  if (trace_eval)
    cout << string((--tabs)*3,' ') << "-> " << res->toString() << endl;
  return res;
}

/** Evaluate an expression AST e under the binding environment env */
Value eval_ ( Expr e, Env env ) {
  if (e->isIntConst())
    return mkIntVal(e->toIntConst()->value);
  else if (e->isCallExp()) {
    auto z = e->toCallExp();
    auto x = env->lookup(z->name);
    if (x->isClosureVal()) {
      auto c = x->toClosureVal();
      if (c->args.size() == z->arguments.size()) {
        int i = 0;
        Env nenv = c->env->clone();
        for ( auto v: z->arguments )
          nenv->insert((c->args)[i++],eval(v,env));
        nenv->begin_scope();
        try { // functions return values by throwing an exception
          eval(c->body,nenv);
        } catch (Value v) {
          nenv->end_scope();
          return v;
        }
        splc_error("Returned from the function "+z->name+" without a value");
     } else splc_error("Number of formal parameters does not match with the number of arguments in: "
                   +e->pretty());
    } else splc_error("Undefined function: "+z->name);
  } else if (e->isFunctionExp()) {
    auto z = e->toFunctionExp();
    vector<string> params;
    for ( auto t: z->formal_params )
      params.push_back(get<0>(t));
    return mkClosureVal(params,z->body,env->clone());
  }

  /* PUT YOUR CODE HERE */

  else if (e->isFloatConst())
    return mkFloatVal(e->toFloatConst()->value);
  else if (e->isStringConst())
    return mkStringVal(e->toStringConst()->value);
  else if (e->isBooleanConst())
    return (e->toBooleanConst()->value) ? true_val : false_val;
  else if (e->isNullExp())
    return mkVoidVal();
  else if (e->isVar()) {
    auto z = e->toVar();
    return env->lookup(z->name);
  } else if (e->isBinOpExp()) {
    auto z = e->toBinOpExp();
    
    // Handle short-circuit boolean operators first
    if (z->op == "and") {
      auto l = eval(z->left, env);
      if (!l->isBooleanVal())
        splc_error("Boolean operation must be applied to booleans: " + e->pretty());
      if (!l->toBooleanVal()->value)
        return false_val;
      auto r = eval(z->right, env);
      if (!r->isBooleanVal())
        splc_error("Boolean operation must be applied to booleans: " + e->pretty());
      return r->toBooleanVal()->value ? true_val : false_val;
    } else if (z->op == "or") {
      auto l = eval(z->left, env);
      if (!l->isBooleanVal())
        splc_error("Boolean operation must be applied to booleans: " + e->pretty());
      if (l->toBooleanVal()->value)
        return true_val;
      auto r = eval(z->right, env);
      if (!r->isBooleanVal())
        splc_error("Boolean operation must be applied to booleans: " + e->pretty());
      return r->toBooleanVal()->value ? true_val : false_val;
    }
    
    // For other operators, evaluate both operands first
    auto left = eval(z->left, env);
    auto right = eval(z->right, env);
    
    if (z->op == "plus" || z->op == "minus" || z->op == "times") {
      if (left->isIntVal() && right->isIntVal()) {
        int l = left->toIntVal()->value;
        int r = right->toIntVal()->value;
        if (z->op == "plus") return mkIntVal(l + r);
        else if (z->op == "minus") return mkIntVal(l - r);
        else return mkIntVal(l * r);
      } else {
        float l = toFloat(left);
        float r = toFloat(right);
        if (z->op == "plus") return mkFloatVal(l + r);
        else if (z->op == "minus") return mkFloatVal(l - r);
        else return mkFloatVal(l * r);
      }
    } else if (z->op == "div") {
      float l = toFloat(left);
      float r = toFloat(right);
      return mkFloatVal(l / r);
    } else if (z->op == "mod") {
      if (!left->isIntVal() || !right->isIntVal())
        splc_error("Modulo must be applied to integers: " + e->pretty());
      int l = left->toIntVal()->value;
      int r = right->toIntVal()->value;
      return mkIntVal(l % r);
    } else if (z->op == "eq" || z->op == "not_equal") {
      bool result;
      if (left->isIntVal() && right->isIntVal())
        result = (left->toIntVal()->value == right->toIntVal()->value);
      else if (left->isFloatVal() || right->isFloatVal())
        result = (toFloat(left) == toFloat(right));
      else if (left->isBooleanVal() && right->isBooleanVal())
        result = (left->toBooleanVal()->value == right->toBooleanVal()->value);
      else if (left->isStringVal() && right->isStringVal())
        result = (left->toStringVal()->value == right->toStringVal()->value);
      else if (left->isVectorVal() && right->isVectorVal())
        result = (left->toVectorVal()->value.data() == right->toVectorVal()->value.data());
      else if (left->isRecordVal() && right->isRecordVal())
        result = (left->toRecordVal()->value.data() == right->toRecordVal()->value.data());
      else
        splc_error("Incompatible types in equality: " + e->pretty());
      if (z->op == "eq")
        return result ? true_val : false_val;
      else
        return result ? false_val : true_val;
    } else if (z->op == "lt" || z->op == "leq" || z->op == "gt" || z->op == "geq") {
      float l = toFloat(left);
      float r = toFloat(right);
      bool result;
      if (z->op == "lt") result = (l < r);
      else if (z->op == "leq") result = (l <= r);
      else if (z->op == "gt") result = (l > r);
      else result = (l >= r);
      return result ? true_val : false_val;
    } else
      splc_error("Unknown binary operator: " + z->op);
  } else if (e->isUnOpExp()) {
    auto z = e->toUnOpExp();
    auto operand = eval(z->operand, env);
    
    if (z->op == "minus") {
      if (operand->isIntVal())
        return mkIntVal(-operand->toIntVal()->value);
      else
        return mkFloatVal(-toFloat(operand));
    } else if (z->op == "not") {
      if (!operand->isBooleanVal())
        splc_error("Not operation must be applied to boolean: " + e->pretty());
      return operand->toBooleanVal()->value ? false_val : true_val;
    } else
      splc_error("Unknown unary operator: " + z->op);
  } else if (e->isArrayDeref()) {
    auto z = e->toArrayDeref();
    auto a = eval(z->array, env);
    auto i = eval(z->index, env);
    if (!i->isIntVal())
      splc_error("Array index must be integer: " + e->pretty());
    if (!a->isVectorVal())
      splc_error("Array indexing can only be done on arrays: " + e->pretty());
    auto index = i->toIntVal()->value;
    if (index < 0 || index >= a->toVectorVal()->value.size())
      splc_error("Array index "+to_string(index)+" is out of bounds in "+e->pretty());
    return a->toVectorVal()->value[index];
  } else if (e->isRecordDeref()) {
    auto z = e->toRecordDeref();
    auto r = eval(z->record, env);
    // Special case: array.length
    if (z->attribute == "length" && r->isVectorVal()) {
      return mkIntVal(r->toVectorVal()->value.size());
    }
    if (!r->isRecordVal())
      splc_error("Record dereferencing can only be done on records: " + e->pretty());
    for (auto t: r->toRecordVal()->value) {
      if (get<0>(t) == z->attribute)
        return get<1>(t);
    }
    splc_error("Record does not have attribute " + z->attribute + " in " + e->pretty());
  } else if (e->isTupleDeref()) {
    auto z = e->toTupleDeref();
    auto t = eval(z->tuple, env);
    if (!t->isVectorVal())
      splc_error("Tuple dereferencing can only be done on tuples: " + e->pretty());
    auto index = z->index;
    if (index < 0 || index >= t->toVectorVal()->value.size())
      splc_error("Tuple index "+to_string(index)+" is out of bounds in "+e->pretty());
    return t->toVectorVal()->value[index];
  } else if (e->isRecordExp()) {
    auto z = e->toRecordExp();
    vector<tuple<string,Value>> components;
    for (auto t: z->components) {
      components.push_back(make_tuple(get<0>(t), eval(get<1>(t), env)));
    }
    return mkRecordVal(components);
  } else if (e->isArrayExp()) {
    auto z = e->toArrayExp();
    vector<Value> elements;
    for (auto elem: z->elements) {
      elements.push_back(eval(elem, env));
    }
    return mkVectorVal(elements);
  } else if (e->isArrayGen()) {
    auto z = e->toArrayGen();
    auto len_val = eval(z->length, env);
    if (!len_val->isIntVal())
      splc_error("Array length must be integer: " + e->pretty());
    int len = len_val->toIntVal()->value;
    if (len < 0)
      splc_error("Array length must be non-negative: " + e->pretty());
    vector<Value> elements;
    for (int i = 0; i < len; i++) {
      elements.push_back(eval(z->value, env));
    }
    return mkVectorVal(elements);
  } else if (e->isTupleExp()) {
    auto z = e->toTupleExp();
    vector<Value> elements;
    for (auto elem: z->elements) {
      elements.push_back(eval(elem, env));
    }
    return mkVectorVal(elements);
  }

  else splc_error("Unrecognized expression: "+e->pretty());
}

/** Replace the content of an Lvalue with a new value */
void assign ( Expr e, Value v, Env env ) {
  if (e->isVar())
    env->replace(e->toVar()->name,v);
  else if (e->isArrayDeref()) {
    auto z = e->toArrayDeref();
    auto a = eval(z->array,env);
    auto i = eval(z->index,env);
    if (!i->isIntVal())
      splc_error("Array index must be integer: " + e->pretty());
    if (!a->isVectorVal())
      splc_error("Array indexing can only be done on arrays: " + e->pretty());
    auto index = i->toIntVal()->value;
    if (index < 0 || index >= a->toVectorVal()->value.size())
      splc_error("Array index "+to_string(index)+" is out of bounds in"+e->pretty());
    a->toVectorVal()->value[index] = v;
  }

  /* PUT YOUR CODE HERE */
  else if (e->isRecordDeref()) {
    // write into a single field inside a record value
    auto z = e->toRecordDeref();
    auto r = eval(z->record, env);
    if (!r->isRecordVal())
      splc_error("Record dereferencing can only be done on records: " + e->pretty());
    for (auto& t: r->toRecordVal()->value) {
      if (get<0>(t) == z->attribute) {
        get<1>(t) = v;
        return;
      }
    }
    splc_error("Record does not have attribute " + z->attribute + " in " + e->pretty());
  } else if (e->isTupleDeref()) {
    // write into one element of a tuple (stored as a vector value)
    auto z = e->toTupleDeref();
    auto t = eval(z->tuple, env);
    if (!t->isVectorVal())
      splc_error("Tuple dereferencing can only be done on tuples: " + e->pretty());
    auto index = z->index;
    if (index < 0 || index >= t->toVectorVal()->value.size())
      splc_error("Tuple index "+to_string(index)+" is out of bounds in "+e->pretty());
    t->toVectorVal()->value[index] = v;
  }

  else splc_error("Wrong l-value: "+e->pretty());
}

void eval_ ( Stmt e, Env env );
void eval ( Stmt e, Env env ) {
  if (trace_eval)
    cout << string((tabs++)*3,' ') << "** " << e->toString() << endl;
  try {
    eval_(e,env);
  } catch (Value s) {
    if (trace_eval)
      cout << string((--tabs)*3,' ') << "-> return " << s->toString() << endl;
    throw s;
  }
  if (trace_eval)
    cout << string((--tabs)*3,' ') << "-> " << endl;
}

/** Evaluate a statement AST under the binding environment env */
void eval_ ( Stmt e, Env env ) {
  if (e->isAssignSt()) {
    auto z = e->toAssignSt();
    assign(z->destination,eval(z->source,env),env);
  } else if (e->isCallSt()) {
    auto z = e->toCallSt();
    auto x = env->lookup(z->name);
    if (x->isClosureVal()) {
      auto c = x->toClosureVal();
      if (c->args.size() == z->arguments.size()) {
        int i = 0;
        Env nenv = c->env->clone();
        for ( auto v: z->arguments )
          nenv->insert((c->args)[i++],eval(v,env));
        nenv->begin_scope();
        try { // functions return values by throwing an exception
          eval(c->body,nenv);
        } catch (Value v) {
          if (!v->isVoidVal())
            splc_error("Returning with a value from a void function: "+v->toString());
        }
        nenv->end_scope();
      } else splc_error("Number of formal parameters does not match with the number of arguments in: "
                   +e->pretty());
    } else splc_error("Undefined function: "+z->name);
  } else if (e->isFuncDef()) {
    auto z = e->toFuncDef();
    vector<string> ps;
    for ( auto fp: z->formal_params )
      ps.push_back(get<0>(fp));
    auto closure = mkClosureVal(ps,z->body,env);
    // the closure environment must contain the function to allow recursion
    env->insert(z->name,closure);
    closure->env = env->clone();
  }

  /* PUT YOUR CODE HERE */

  else if (e->isVarDef()) {
    // evaluate the initializer and bind the new variable in the current environment
    auto z = e->toVarDef();
    Value val = eval(z->value, env);
    env->insert(z->name, val);
  } else if (e->isBlockSt()) {
    // run a sequence of statements in their own scope
    auto z = e->toBlockSt();
    env->begin_scope();
    for (auto s: z->stmts) {
      eval(s, env);
      // if a loop asked to exit, stop executing the rest of this block
      if (loop_exit) {
        break;
      }
    }
    env->end_scope();
  } else if (e->isReadSt()) {
    auto z = e->toReadSt();
    for (auto lval: z->arguments) {
      string input;
      cin >> input;
      Value val;
      if (lval->isVar()) {
        // try to infer the type of an existing variable; fall back to int
        try {
          auto existing = env->lookup(lval->toVar()->name);
          if (existing->isFloatVal()) {
            val = mkFloatVal(stof(input));
          } else {
            val = mkIntVal(stoi(input));
          }
        } catch (...) {
          val = mkIntVal(stoi(input));
        }
      } else {
        // non-variable l-values are always read as ints
        val = mkIntVal(stoi(input));
      }
      assign(lval, val, env);
    }
  } else if (e->isPrintSt()) {
    auto z = e->toPrintSt();
    for (size_t i = 0; i < z->arguments.size(); i++) {
      Value v = eval(z->arguments[i], env);
      if (v->isStringVal())
        cout << v->toStringVal()->value;
      else
        cout << toString(v);
      if (i < z->arguments.size() - 1)
        cout << " ";
    }
    cout << endl;
  } else if (e->isIfSt()) {
    auto z = e->toIfSt();
    auto cond = eval(z->condition, env);
    if (!cond->isBooleanVal())
      splc_error("If condition must be boolean: " + e->toString());
    if (cond->toBooleanVal()->value) {
      eval(z->then_stmt, env);
    } else if (z->else_stmt) {
      eval(z->else_stmt, env);
    }
  } else if (e->isWhileSt()) {
    auto z = e->toWhileSt();
    while (true) {
      auto cond = eval(z->condition, env);
      if (!cond->isBooleanVal())
        splc_error("While condition must be boolean: " + e->toString());
      if (!cond->toBooleanVal()->value)
        break;
      eval(z->body, env);
      if (loop_exit) {
        loop_exit = false;
        break;
      }
    }
  } else if (e->isLoopSt()) {
    auto z = e->toLoopSt();
    while (true) {
      eval(z->body, env);
      if (loop_exit) {
        loop_exit = false;
        break;
      }
    }
  } else if (e->isForSt()) {
    auto z = e->toForSt();
    auto initial_val = eval(z->initial, env);
    auto last_val = eval(z->last, env);
    auto increment_val = eval(z->increment, env);
    if (!initial_val->isIntVal() || !last_val->isIntVal() || !increment_val->isIntVal())
      splc_error("For loop expressions must be integers: " + e->toString());
    int initial = initial_val->toIntVal()->value;
    int last = last_val->toIntVal()->value;
    int increment = increment_val->toIntVal()->value;
    if (increment == 0)
      splc_error("For loop increment cannot be zero: " + e->toString());
    // set the loop variable to the starting value
    env->replace(z->variable, mkIntVal(initial));
    while (true) {
      auto current = env->lookup(z->variable);
      int current_val = current->toIntVal()->value;
      // stop when the counter goes past the final bound
      if ((increment > 0 && current_val > last) || (increment < 0 && current_val < last))
        break;
      eval(z->body, env);
      if (loop_exit) {
        loop_exit = false;
        break;
      }
      env->replace(z->variable, mkIntVal(current_val + increment));
    }
    // keep the final value of the loop variable after the loop ends
  } else if (e->isExitSt()) {
    loop_exit = true;
  } else if (e->isReturnValueSt()) {
    auto z = e->toReturnValueSt();
    Value v = eval(z->value, env);
    throw v;
  } else if (e->isReturnSt()) {
    throw mkVoidVal();
  } else if (e->isTypeDef()) {
    // Type definitions are ignored at runtime
    return;
  }

  else splc_error("Unrecognized statement: "+e->toString());
}

void eval ( Stmt e ) {
  eval(e,global_env);
}

/** Evaluate the main program using the interpreter */
void eval ( Program e ) {
  eval(e->body);
}

// End of work: interpreter.cpp - 11/18/2025 - Nabih