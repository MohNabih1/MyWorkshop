//Mohamad Nabih Alkhateeb
//Student ID: 1002170786
//CSE 4305: Project 1
//Print.cpp
//09/08/2025

/* 
 __ __       _    _ _       _    _  _   
|  \  \ ___ | |_ | \ | ___ | |_ <_>| |_ 
|     |/ . \| . ||   |<_> || . \| || . |
|_|_|_|\___/|_|_||_\_|<___||___/|_||_|_|

*/

/*
This program implements the SPL expression printer for Project #1. 
Its goal is to convert expression ASTs back into SPL code with minimal 
parentheses while preserving correctness.

Steps:
1. Define operator precedences and their surface symbols.
2. Recursively traverse the AST using dynamic_cast to detect node types.
3. Print literals, variables, unary/binary ops, and aggregates.
4. Apply precedence/associativity rules to insert parentheses only when needed.
5. Escape string literals to ensure valid SPL syntax.
*/



#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include "Node.h"
#include "AST.h"

using std::string; using std::map; using std::ostringstream; using std::to_string; using std::vector; using std::get;

map<string,int> precedence{ {"or",0},
                            {"and",1},
                            {"not",2},
                            {"leq",3}, {"geq",3}, {"lt",3}, {"gt",3}, {"eq",3}, {"neq",3},
                            {"plus",4}, {"minus",4},
                            {"times",5}, {"div",5}, {"mod",5} };

map<string,string> opr{ {"plus","+"}, {"minus","-"}, {"times","*"}, {"div","/"}, {"mod","%"},
                        {"leq","<="}, {"geq",">="}, {"lt","<"}, {"gt",">"}, {"eq","=="},
                        {"neq","<>"}, {"and","&&"}, {"or","||"}, {"not","not"} };


/* print the expression AST e into a string;
   prec is the max operator precedence for embedding e in parentheses */
string print ( Expr e, int prec ) {
  /* put your code here */
  if (!e) return "?";                    //null expression

  // precedence level for postfix constructs 
  const int postfix_lvl = []{
    int best = 0;
    for (const auto& p : precedence)     // scan table to get max precedence
      best = std::max(best, p.second);
    return best + 1;                     // set tighter than any operator
  }();

  // map internal op names to surface symbols 
  auto symb = [&](const string& op) -> const string& {
    auto it = opr.find(op);
    return it!=opr.end() ? it->second : op;
  };

  // only add parentheses when needed 

  auto wrapIf = [&](const string& s, int mine, int ctx) -> string {
    return (mine < ctx) ? ("(" + s + ")") : s;
  };

  // precedence tier used for unary ops
  auto unaryTier = [&]() -> int {
    return precedence.at("not");
  };

  // enforce right associativity for non-associative and relational operators
  auto rhsNeedsBump = [&](const string& op) -> bool {
    return op=="minus" || op=="div" || op=="mod" ||
           op=="leq" || op=="geq" || op=="lt" || op=="gt" || op=="eq" || op=="neq";
  };

  // quote and escape a string literal
  auto quoteIt = [&](const string& s) -> string {
    string buf; buf.reserve(s.size()+4);
    buf.push_back('"');
    for (char c : s) {
      switch (c) {
        case '\\': buf += "\\\\"; break;
        case '\"': buf += "\\\""; break;
        case '\n': buf += "\\n";  break;
        case '\t': buf += "\\t";  break;
        case '\r': buf += "\\r";  break;
        default: buf.push_back(c); break;
      }
    }
    buf.push_back('"');
    return buf;
  };

  // literals and simple atoms
   if (auto v  = dynamic_cast<Var_*>(e))          return v->name;
 if (auto ic = dynamic_cast<IntConst_*>(e))     return to_string(ic->value);
  if (auto fc = dynamic_cast<FloatConst_*>(e))   { ostringstream os; os << fc->value; return os.str(); }
   if (auto sc = dynamic_cast<StringConst_*>(e))  return quoteIt(sc->value);
  if (auto bc = dynamic_cast<BooleanConst_*>(e)) return bc->value ? "true" : "false";
  if (dynamic_cast<NullExp_*>(e))                return "()";

  // for unary expressions
  if (auto u = dynamic_cast<UnOpExp_*>(e)) {
     int    mine   = unaryTier();
    string inner  = print(u->operand, mine);
    string outStr = (u->op == "not")
                      ? (symb(u->op) + string(" ") + inner)
                      : (string("-") + inner);
      return wrapIf(outStr, mine, prec);
  }

  // for binary expressions 
  if 
   (auto b = dynamic_cast<BinOpExp_*>(e)) {
    int mine   = precedence[b->op];
    int rhsCtx = mine + (rhsNeedsBump(b->op) ? 1 : 0);   // tighten right side when needed
    string Ls  = print(b->left,  mine);
    string Rs  = print(b->right, rhsCtx);
    string outStr = Ls + " " + symb(b->op) + " " + Rs;
     return wrapIf(outStr, mine, prec);
  }

  // postfix group
  if (auto ad = dynamic_cast<ArrayDeref_*>(e)) {
    string base = print(ad->array, postfix_lvl);
    string idx  = print(ad->index, 0);
    return base + "[" + idx + "]";
  }

  if (auto rd = dynamic_cast<RecordDeref_*>(e)) {
    string base = print(rd->record, postfix_lvl);
    return base + "." + rd->attribute;
  }
  if (auto td = dynamic_cast<TupleDeref_*>(e)) {
    string base = print(td->tuple, postfix_lvl);
    return base + "#" + to_string(td->index);
  }

  
  if (auto c = dynamic_cast<CallExp_*>(e)) {
    string buf = c->name + "(";
    for (size_t i = 0; i < c->arguments.size(); ++i) {
      if (i) buf += ", ";
      buf += print(c->arguments[i], 0);
    }
    buf += ")";
    return buf;
  }

  // aggregates
  if (auto re = dynamic_cast<RecordExp_*>(e)) {
    string buf = "{";
    for (size_t i = 0; i < re->components.size(); ++i) {
      if (i) buf += ", ";
      buf += get<0>(re->components[i]) + "=" + print(get<1>(re->components[i]), 0);
    }
    buf += "}";
    return buf;
  }
  
  if  (auto ae = dynamic_cast<ArrayExp_*>(e)) {
    string buf = "[";
    for (size_t i = 0; i < ae->elements.size(); ++i) {
      if (i) buf += ", ";
      buf += print(ae->elements[i], 0);
    }
    buf += "]";
    return buf;
  }
  if (auto ag = dynamic_cast<ArrayGen_*>(e)) {
    return "array(" + print(ag->length, 0) + ", " + print(ag->value, 0) + ")";
  }

    if (auto te = dynamic_cast<TupleExp_*>(e)) {
    string buf = "(";
    for (size_t i = 0; i < te->elements.size(); ++i) {
      if (i) buf += ", ";
      buf += print(te->elements[i], 0);
    }
    buf += ")";
    return buf;
  }

  return "?"; 
}

string print ( Expr e ) {
  return print(e,0);
}
// End of Print.cpp - 09/09/2025 - Nabih