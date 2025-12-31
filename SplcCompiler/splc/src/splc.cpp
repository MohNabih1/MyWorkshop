/*
 * Copyright © 2025 Leonidas Fegaras, University of Texas at Arlington
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "AST.h"
#include <FlexLexer.h>
#include "spl.parser.h"
#include "scanner.h"
#include <readline/readline.h>
#include <readline/history.h>

// Compatibility layer for __isoc23_strtol
extern "C" {
    long int __isoc23_strtol(const char *nptr, char **endptr, int base) {
        return strtol(nptr, endptr, base);
    }
}

int screen_size = 120;
int tabs = 0;
int position_from_start = 0;
bool print_ast = false;
bool print_ir = false;
bool trace_scanner = false;
bool trace_parser = false;
bool trace_typecheck = false;
bool trace_codegen = false;
bool trace_eval = false;
bool evaluate = false;
int project = -1;
string malloc_op = "GC_malloc";
string spl_home;
extern int yydebug;
extern Environment<Declaration>* st;
extern Environment<Value>* global_env;
extern Program program;

string print ( Expr e );
void typecheck ( Program e );
void typecheck ( Stmt e, Type expected_type );
list<tuple<string,IRdecl>*>* code  ( Program e );
void emit ( list<tuple<string,IRdecl>*>* decls, const string &llvm_file );
void eval ( Program e );
void eval ( Stmt e );
string toString ( Value v );
string toString ( Type v );

using namespace splc;

void scan_file ( const string &file_name ) {
  line_number = 1;
  position = 1;
  ifstream file;
  file.open(file_name);
  if (!file.is_open())
    throw runtime_error("I can't open the file "+file_name);
  Scanner scanner{ file, cerr };
  Parser::semantic_type yylval;
  while (scanner.lex(&yylval) != Parser::token::YYEOF)
    continue;
  file.close();
}

Program parse_file ( const string &file_name ) {
  line_number = 1;
  position = 1;
  ifstream file;
  file.open(file_name);
  if (!file.is_open())
    throw runtime_error("I can't open the file "+file_name);
  Scanner scanner{ file, cerr };
  Parser parser{ &scanner };
  parser.set_debug_level(trace_parser ? 1 : 0);
  parser.parse();
  file.close();
  return program;
}

void scan ( const string &line ) {
  istringstream ss(line);
  Scanner scanner{ ss, cerr };
  Parser::semantic_type yylval;
  while (scanner.lex(&yylval) != Parser::token::YYEOF)
    continue;
}

Program parse ( const string &line ) {
  istringstream ss(line);
  Scanner scanner{ ss, cerr };
  Parser parser{ &scanner };
  parser.set_debug_level(trace_parser ? 1 : 0);
  parser.parse();
  return program;
}

/* read multiple lines until all (), {}, and [] are matched */
string readl () {
  bool exit = false;
  string s = "";
  while (!exit) {
    char* line = readline("> ");
    position_from_start++;
    string sline = string(line);
    int i = 0;
    s += sline;
    for ( char c: s )
      if (c == '{' || c == '(' || c == '[') i++;
      else if (c == '}' || c == ')' || c == ']') i--;
    free(line);
    exit = (i == 0) || sline == "";
    if (!exit) s += "\n";
  }
  return s;
}

/* read-eval-print loop (REPL) to evaluate SPL expressions and statements interactively */
void repl () {
  static int rc = 0;
  cout << "SPL interpreter: type `quit` or `q` to exit, `=expr` to evaluate "
       << "an SPL expression, or `stmt` to evaluate an SPL statement\n";
  st->begin_scope();
  global_env->begin_scope();
  while (true) {
    string line { readl() };
    if (line == "quit" || line == "q")
      break;
    if (line == "")
      continue;
    try {
      auto stmt { line };
      add_history(line.c_str());
      if (project == 2) {
        scan(line);
        continue;
      }
      if (line[0] == '=') {
        rc++;
        stmt = "var res"+to_string(rc)+line+";";
      } else if (line[line.size()-1] != ';')
        stmt += ";";
      line_number = 1;
      position = 1;
      if (line[0] == '=')
        position = -(to_string(rc).size()+8);
      auto program { parse(stmt) };
      Stmt x { program->body };
      if (program->body->toBlockSt()->stmts.size() == 1)
        x = program->body->toBlockSt()->stmts[0];
      if (project <= 0 || project > 4)
        typecheck(x,mkNoType());
      if (line[0] == '=') {
        auto z = x->toVarDef()->value;
        if (project == 1 || project == 3 || project == 4 || print_ast)
          cout << "AST:\n" << z->pretty("  ") << endl;
        if (project <= 0 || project > 3)
          cout << "res" << rc << ": "
               << toString(z->tpe) << endl;
        if (project == 1)
          cout << "Print: " << print(z) << endl;
      } else if (project == 3 || project == 4 || print_ast)
        cout << "AST:\n" << x->pretty("  ") << endl;
      if (project <= 0 || project == 6)
        eval(x);
      if ((project <= 0 || project == 6) && line[0] == '=') {
        int te = trace_eval; trace_eval = false;
        cout << "res" << rc << " = ";
        eval(parse("print(res"+to_string(rc)+");"));
        trace_eval = te;
      }
      if (project == 7) {
        auto decls { code(program) };
        cout << "IR code:\n";
        for ( auto d: *decls )
          cout << get<1>(*d)->pretty("  ") << endl;
      }
    } catch ( const runtime_error &ex ) {
      cerr << "*** " << ex.what() << endl;
    } catch (...) {
      cerr << "*** run-time error\n";
    }
  }
}

string get_parent_dir ( const string &path ) {
  auto p(path);
  auto it = p.rbegin();
  int i = 0;
  while (*it != '/') { it++; i++; };
  p.replace(p.length()-i,i,"..");
  return p;
}

int main ( int argc, char* argv[] ) {
  try {
    trace_parser = false; 
    spl_home = get_parent_dir(string(argv[0]));
    vector<string> files;
    for ( int i = 1; i < argc; i++ ) {
      string file = argv[i];
      if (file == "-project") {
        if (i+1 < argc)
          project = atoi(argv[++i]);
        else splc_error("Expected a project number");
        if (project == 2)
          trace_scanner = true;
        else if (project == 5)
          trace_typecheck = true;
        else if (project == 6)
          trace_eval = true;
        else if (project == 7)
          trace_codegen = true;
        else if (project != 1 && project != 3 && project != 4)
          splc_error("Wrong project number: "+argv[i]);
      } else if (file == "-trace") {
        if (i+1 < argc && string(argv[i+1]) == "scanner")
          { trace_scanner = true; i++; }
        else if (i+1 < argc && string(argv[i+1]) == "parser")
          { trace_parser = true; i++; }
        else if (i+1 < argc && string(argv[i+1]) == "typecheck")
          { trace_typecheck = true; i++; }
        else if (i+1 < argc && string(argv[i+1]) == "codegen")
          { trace_codegen = true; i++; }
        else if (i+1 < argc && string(argv[i+1]) == "eval")
          { trace_eval = true; i++; }
        else splc_error("You can only trace scanner, parser, typecheck, codegen, and eval");
      } else if (file == "-show") {
        if (i+1 < argc && string(argv[i+1]) == "ast")
          { print_ast = true; i++; }
        else if (i+1 < argc && string(argv[i+1]) == "ir")
          { print_ir = true; i++; }
        else splc_error("You can only show ast and ir");
      } else if (file == "-eval")
        evaluate = true;
      else if (file == "-nogc")
        malloc_op = "malloc";
      else if (file[0] == '-')
        splc_error("Unknown parameter: "+file);
      else files.push_back(file);
    }
    if (files.size() == 0)
      repl();
    else if (project != 1)
      for ( string file: files ) {
          cout << "Compiling the file: " << file << endl;
          if (project == 2) {
            scan_file(file);
            continue;
          }
          auto program = parse_file(file);
          if (project == 3 || project == 4 || print_ast)
            cout << "AST:\n" << program->pretty("  ") << endl;
          if (project < 0 || project >= 5)
            typecheck(program);
          if (project == 6 || evaluate) {
            cout << "Evaluating using the SPL interpreter:\n";
            eval(program);
          } else if (project < 0 || project == 7) {
            auto decls { code(program) };
            if (print_ir) {
              cout << "IR code:\n";
              for ( auto d: *decls )
                cout << get<1>(*d)->pretty("  ") << endl;
            }
            string lfile { file.replace(file.length()-4,4,".ll") };
            cout << "Generating the LLMV file: " << lfile << endl;
            emit(decls,lfile);
          }
      }
    } catch ( const runtime_error &ex ) {
        cerr << "*** " << ex.what() << endl;
        exit(-1);
    }
}
