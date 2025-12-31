//Mohamad Nabih Alkhateeb
//Student ID: 1002170786 
//CSE 4305: Project 4
//spl.y
//10/08/2025

/*  
  __ __       _    _ _       _    _  _   
  |  \  \ ___ | |_ | \ | ___ | |_ <_>| |_ 
  |     |/ . \| . ||   |<_> || . \| || . |
  |_|_|_|\___/|_|_||_\_|<___||___/|_||_|_|

 */ 


/* 
   SPL Parser (Bison Grammar)

   - This file defines the parser for the SPL language. It uses Bison to translate
   SPL expressions and statements into Abstract Syntax Trees (ASTs). The grammar
   handles variables, arithmetic and boolean operations, arrays, tuples, records,
   function calls, and anonymous functions. Operator precedence and associativity
   rules are included to remove common parser conflicts. 

   - The parser works together with the SPL scanner and AST definitions
   to satisfy Project 3 requirements.

   - spl.y has been tested with the solution scanner and the Makefile has been modified to reflect that:
   scanner =  ${solution-dir}/spl.scanner.o  #spl.scanner.o " on line 12 of the Makefile.

   - I had to use the solution scanner to make sure the output of the testes when ran with my spl.y exactly
  matches the expected output of the solution ( even though my scanner was correct, when tested my parser with it, the
  output did not exactly match and it had a few minor differences).


*/

/*   PROJECT 4: 
How I approached the tricky parts

Function types vs. parentheses:
      - The sequence “(T) -> U” was previously mis-read as just “(T)”.
      - I added a precedence symbol LOWER_THAN_ARROW and gave (type) that
        lower precedence. This forces the parser to SHIFT on '->' and build
        a function type when it’s there, while still allowing plain (type)
        when no arrow follows.
      - I split function types into three explicit forms: 0-arg, 1-arg,
        and 2+-arg. That removes ambiguity and makes cases like
        (int,(int,float))->array[{...}] parse exactly as expected.

Dangling else:
      - Resolved with a precedence tie-breaker: the “if ... stmt” rule
        uses %prec LOWER_THAN_ELSE so the parser prefers shifting ELSE.

Optional semicolons & blocks:
      - Kept both “block” and “block SEMI” as statements.
      - The block rule is a single form: { stmt_or_defs }, with an empty
        list allowed. That removes the RB shift/reduce around “{}”.

Major problems I ran into and how did i fix them: 
   - Syntax errors at token '->' in many type declarations:
     caused by reducing (type) too early. Fixed with LOWER_THAN_ARROW and
     explicit function-type rules.
   - Occasional conflict on '}':
     simplified “block” to a single rule and let the list be empty.
   - Dangling else ambiguity:
     fixed with LOWER_THAN_ELSE on the no-else rule.

   
   */

%{
#include "Node.h"
#include "AST.h"
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <FlexLexer.h>
%}

%require "3.7.4"
%language "C++"
%defines "include/spl.parser.h"
%output "src/spl.parser.cpp"
%define api.parser.class {Parser}
%define api.namespace {splc}
%define api.value.type variant
%parse-param {Scanner* scanner}
%debug

%{
Program program;


template< typename T >
vector<T> elem ( T x ) {
  return vector<T>({ x });
}

Stmt empty_block = mkBlockSt(vector<Stmt>());
%}
 
%code requires {
#include "Node.h"
#include "AST.h"

namespace splc { class Scanner; }
}

%code {
#include "scanner.h"
#define yylex(x) scanner->lex(x)
}

/* terminals */

%token AND ARRAY BOOLEAN BY DEF DIV ELSE EQUAL EXIT FALSE FLOAT FOR FUNCTION
%token IF INT LOOP MOD NOT OR PRINT READ RETURN STRING TO TRUE TYPE VAR WHILE
%token PLUS MINUS TIMES LT LEQ GT GEQ EQ NEQ COLON SEMI ARROW
%token COMMA SHARP DOT LP RP LB RB LSB RSB UMINUS

/* terminals with values */


%token ID STRING_LITERAL INTEGER_LITERAL FLOAT_LITERAL
%type <string>  ID STRING_LITERAL
%type <int>     INTEGER_LITERAL
%type <float>   FLOAT_LITERAL

/* precedences and associativities*/

%nonassoc LOWER_THAN_ARROW
%right ARROW
%left OR
%left AND
%nonassoc EQ NEQ
%nonassoc LT LEQ GT GEQ
%left PLUS MINUS
%left TIMES DIV MOD
%right UMINUS NOT
/* dangling-else: prefer shifting ELSE over reducing the no-else form */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

//non terminals
%start program
%nterm <Program>                    program
%nterm <Stmt>                       stmt
%nterm <Expr>                       expr postfix primary lvalue
%nterm <Type>                       type
%nterm <vector<Expr>>               exprs exprs_opt
%nterm <tuple<string,Type>>         param
%nterm <vector<tuple<string,Type>>> param_list param_list_opt record_type_components
%nterm <vector<Type>>               tuple_types
%nterm <vector<tuple<string,Expr>>> record_components
%nterm <vector<Stmt>>               block
%nterm <vector<Expr>>               lvalues
%nterm <Stmt>                       stmt_or_def
%nterm <vector<Stmt>>               stmt_or_defs

%%

program
  : stmt_or_defs                            { $$ = mkProgram(mkBlockSt($1)); program = $$; }
  ;

stmt_or_defs
  : /* empty */                             { $$ = vector<Stmt>(); }
  | stmt_or_defs stmt_or_def                { $$ = $1; $$.push_back($2); }
  ;

stmt_or_def
  : stmt                                    { $$ = $1; }
  | VAR ID COLON type EQUAL expr SEMI       { $$ = mkVarDef($2,$4,$6); }
  | VAR ID COLON type SEMI                  { $$ = mkVarDef($2,$4,mkNullExp()); }
  | TYPE ID EQUAL type SEMI                 { $$ = mkTypeDef($2,$4); }
  | DEF ID LP param_list_opt RP COLON type block SEMI
                                            { $$ = mkFuncDef($2,$4,$7,mkBlockSt($8)); }
  | DEF ID LP param_list_opt RP COLON type block
                                            { $$ = mkFuncDef($2,$4,$7,mkBlockSt($8)); }
  | DEF ID LP param_list_opt RP block SEMI  { $$ = mkFuncDef($2,$4,mkNoType(),mkBlockSt($6)); }
  | DEF ID LP param_list_opt RP block       { $$ = mkFuncDef($2,$4,mkNoType(),mkBlockSt($6)); }
  ;

stmt
  : lvalue EQUAL expr SEMI                  { $$ = mkAssignSt($1,$3); }
  | VAR ID EQUAL expr SEMI                  { $$ = mkVarDef($2,mkAnyType(),$4); }
  | ID LP exprs RP SEMI                     { $$ = mkCallSt($1,$3); }
  | ID LP RP SEMI                           { $$ = mkCallSt($1,vector<Expr>()); }
  | block                                   { $$ = mkBlockSt($1); }
  | block SEMI                              { $$ = mkBlockSt($1); }  
  | READ LP lvalues RP SEMI                 { $$ = mkReadSt($3); }
  | PRINT LP exprs_opt RP SEMI              { $$ = mkPrintSt($3); }
  | IF LP expr RP stmt %prec LOWER_THAN_ELSE{ $$ = mkIfSt($3,$5,mkBlockSt(vector<Stmt>())); }
  | IF LP expr RP stmt ELSE stmt            { $$ = mkIfSt($3,$5,$7); }
  | FOR LP ID EQUAL expr TO expr RP stmt    { $$ = mkForSt($3,$5,$7,mkIntConst(1),$9); }
  | FOR LP ID EQUAL expr TO expr BY expr RP stmt
                                            { $$ = mkForSt($3,$5,$7,$9,$11); }
  | WHILE LP expr RP stmt                   { $$ = mkWhileSt($3,$5); }
  | LOOP stmt                               { $$ = mkLoopSt($2); }
  | EXIT SEMI                               { $$ = mkExitSt(); }
  | RETURN expr SEMI                        { $$ = mkReturnValueSt($2); }
  | RETURN SEMI                             { $$ = mkReturnSt(); }
  ;

lvalue
  : ID                                      { $$ = mkVar($1); }
  | lvalue LSB expr RSB                     { $$ = mkArrayDeref($1, $3); }
  | lvalue DOT ID                           { $$ = mkRecordDeref($1, $3); }
  | lvalue SHARP INTEGER_LITERAL            { $$ = mkTupleDeref($1, $3); }
  ;

expr
  : postfix                                 { $$ = $1; }
  | MINUS expr %prec UMINUS                 { $$ = mkUnOpExp("minus", $2); }
  | NOT expr                                { $$ = mkUnOpExp("not", $2); }
  | expr PLUS  expr                         { $$ = mkBinOpExp("plus",  $1, $3); }
  | expr MINUS expr                         { $$ = mkBinOpExp("minus", $1, $3); }
  | expr TIMES expr                         { $$ = mkBinOpExp("times", $1, $3); }
  | expr DIV   expr                         { $$ = mkBinOpExp("div",   $1, $3); }
  | expr MOD   expr                         { $$ = mkBinOpExp("mod",   $1, $3); }
  | expr AND   expr                         { $$ = mkBinOpExp("and",   $1, $3); }
  | expr OR    expr                         { $$ = mkBinOpExp("or",    $1, $3); }
  | expr EQ    expr                         { $$ = mkBinOpExp("equal",     $1, $3); }
  | expr NEQ   expr                         { $$ = mkBinOpExp("not_equal", $1, $3); }
  | expr LT    expr                         { $$ = mkBinOpExp("lt",    $1, $3); }
  | expr LEQ   expr                         { $$ = mkBinOpExp("leq",   $1, $3); }
  | expr GT    expr                         { $$ = mkBinOpExp("gt",    $1, $3); }
  | expr GEQ   expr                         { $$ = mkBinOpExp("geq",   $1, $3); }
  | ARRAY LP expr COMMA expr RP             { $$ = mkArrayGen($3, $5); }
  | ID LP exprs RP                          { $$ = mkCallExp($1, $3); }
  | ID LP RP                                { $$ = mkCallExp($1, vector<Expr>()); }
  | FUNCTION LP param_list_opt RP COLON type block
                                            { $$ = mkFunctionExp($3, $6, mkBlockSt($7)); }
  | FUNCTION LP param_list_opt RP block     { $$ = mkFunctionExp($3, mkNoType(), mkBlockSt($5)); }
  | LB record_components RB                 { $$ = mkRecordExp($2); }   /* non-empty record literal */
  | LSB exprs RSB                           { $$ = mkArrayExp($2); }
  | LP exprs_opt RP                         { 
      if ($2.size() == 0) $$ = mkNullExp();
      else if ($2.size() == 1) $$ = $2[0];
      else $$ = mkTupleExp($2);
    }
  ;

postfix
  : primary
  | postfix LSB expr RSB                    { $$ = mkArrayDeref($1, $3); }
  | postfix DOT ID                          { $$ = mkRecordDeref($1, $3); }
  | postfix SHARP INTEGER_LITERAL           { $$ = mkTupleDeref($1, $3); }
  ;

primary
  : ID                                      { $$ = mkVar($1); }
  | INTEGER_LITERAL                         { $$ = mkIntConst($1); }
  | FLOAT_LITERAL                           { $$ = mkFloatConst($1); }
  | STRING_LITERAL                          { $$ = mkStringConst($1); }
  | TRUE                                    { $$ = mkBooleanConst(true); }
  | FALSE                                   { $$ = mkBooleanConst(false); }
  ;

exprs
  : expr                                    { $$ = elem($1); }
  | exprs COMMA expr                        { $$ = $1; $$.push_back($3); }
  ;

exprs_opt
  : /* empty */                             { $$ = vector<Expr>(); }
  | exprs                                   { $$ = $1; }
  ;

param
  : ID COLON type                           { $$ = make_tuple($1, $3); }
  ;

param_list
  : param                                   { $$ = elem($1); }
  | param_list COMMA param                  { $$ = $1; $$.push_back($3); }
  ;

param_list_opt
  : /* empty */                             { $$ = vector<tuple<string,Type>>(); }
  | param_list                              { $$ = $1; }
  ;

record_components
  : ID EQUAL expr                           { $$ = elem(make_tuple($1, $3)); }
  | record_components COMMA ID EQUAL expr   { $$ = $1; $$.push_back(make_tuple($3, $5)); }
  ;

//types

type
  // function types: explicit 0-arg, 1-arg, and 2+-arg forms 
  : LP RP ARROW type                          { $$ = mkFunctionType(vector<Type>(), $4); }               
  | LP type RP ARROW type                     { $$ = mkFunctionType(elem($2), $5); }                     
  | LP type COMMA tuple_types RP ARROW type   {                                                          
      vector<Type> ps = elem($2);
      ps.insert(ps.end(), $4.begin(), $4.end());
      $$ = mkFunctionType(ps, $7);
    }

  // void and tuples
  | LP RP                                     { $$ = mkNoType(); }                                       
  | LP type COMMA tuple_types RP              {                                                          
      vector<Type> ts = elem($2);
      ts.insert(ts.end(), $4.begin(), $4.end());
      $$ = mkTupleType(ts);
    }

  // parenthesized type
  | LP type RP %prec LOWER_THAN_ARROW         { $$ = $2; }                                               

  // base / named / array types 


  | INT                                       { $$ = mkIntType(); }
  | FLOAT                                     { $$ = mkFloatType(); }
  | STRING                                    { $$ = mkStringType(); }
  | BOOLEAN                                   { $$ = mkBooleanType(); }
  | ID                                        { $$ = mkNamedType($1); }
  | ARRAY LSB type RSB                        { $$ = mkArrayType($3); }

  // non-empty record type
  | LB record_type_components RB              { $$ = mkRecordType($2); }
  ;

// non empty record types fields
record_type_components
  : ID COLON type                           { $$ = elem(make_tuple($1, $3)); }
  | record_type_components COMMA ID COLON type
                                            { $$ = $1; $$.push_back(make_tuple($3, $5)); }
  ;

// one-or-more types; used for function parameter lists and tuple tails 
tuple_types
  : type                                    { $$ = elem($1); }
  | tuple_types COMMA type                  { $$ = $1; $$.push_back($3); }
  ;

lvalues
  : lvalue                                  { $$ = elem($1); }
  | lvalues COMMA lvalue                    { $$ = $1; $$.push_back($3); }
  ;


block
  : LB stmt_or_defs RB                      { $$ = $2; }
  ;

%%

void splc::Parser::error ( const string &msg ) {
  throw runtime_error(msg+" ("+to_string(line_number)+":"+
       to_string(position)+") at token '"+scanner->YYText()+"'");
}

// End of work: spl.y - 10/08/2025 - Nabih
