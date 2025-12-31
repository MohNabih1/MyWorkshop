%{
/* Mohamad Nabih Alkhateeb
   Student ID:1002170786
   CSE 4305: Project 2
   spl.lex
   09/18/2025

   __ __       _    _ _       _    _  _   
  |  \  \ ___ | |_ | \ | ___ | |_ <_>| |_ 
  |     |/ . \| . ||   |<_> || . \| || . |
  |_|_|_|\___/|_|_||_\_|<___||___/|_||_|_|

    This scanner for SPL: recognizes keywords, identifiers, literals,
   operators, and punctuation; skips whitespace and comments. Undefined characters return an error.
   Returns tokens defined in spl.y.
*/
#include <string>
#include <iostream>
#include "spl.parser.h"
#include "scanner.h"

#define YY_DECL int splc::Scanner::lex ( splc::Parser::semantic_type* yylval )

int line_number = 1;
int position = 0;

extern bool trace_scanner;

inline int _token ( int id, const string &name, const char* text, int length ) {
  position += length;
  if (trace_scanner)
    cout << line_number << ":" << position << "\t" << text
	 << "\t" << string(name) << endl;
  return id;
}

#define token(k) _token(Parser::token::k,#k,yytext,yyleng)

void scanner_error ( const string &msg ) {
  throw runtime_error("Scanner error at "+to_string(line_number)+":"
		      +to_string(position)+": "+msg);
}
%}

%option c++ noyywrap

INT      [0-9]+
/* Floats must have digits before the dot; allow optional digits after the dot */
FLOAT    [0-9]+\.[0-9]*([eE][+-]?[0-9]+)?
ID       [a-zA-Z_][a-zA-Z0-9_]*
STRING   \"([^\"\\]|\\.)*\"

%x COMMENT

%%

"array"             { return token(ARRAY); }
"boolean"           { return token(BOOLEAN); }
"def"               { return token(DEF); }
"else"              { return token(ELSE); }
"exit"              { return token(EXIT); }
"false"             { return token(FALSE); }
"float"             { return token(FLOAT); }
"for"               { return token(FOR); }
"function"          { return token(FUNCTION); }
"if"                { return token(IF); }
"int"               { return token(INT); }
"loop"              { return token(LOOP); }
"not"               { return token(NOT); }
"print"             { return token(PRINT); }
"read"              { return token(READ); }
"return"            { return token(RETURN); }
"string"            { return token(STRING); }
"to"                { return token(TO); }
"true"              { return token(TRUE); }
"type"              { return token(TYPE); }
"var"               { return token(VAR); }
"while"             { return token(WHILE); }
"by"                { return token(BY); }
"+"                 { return token(PLUS); }
"-"                 { return token(MINUS); }
"*"                 { return token(TIMES); }
"/"                 { return token(DIV); }
"%"                 { return token(MOD); }
"<"                 { return token(LT); }
"<="                { return token(LEQ); }
">"                 { return token(GT); }
">="                { return token(GEQ); }
"=="                { return token(EQ); }
"<>"                { return token(NEQ); }
"&&"                { return token(AND); }
"||"                { return token(OR); }
"!"                 { return token(NOT); }
"("                 { return token(LP); }
")"                 { return token(RP); }
"["                 { return token(LSB); }
"]"                 { return token(RSB); }
"{"                 { return token(LB); }
"}"                 { return token(RB); }
","                 { return token(COMMA); }
";"                 { return token(SEMI); }
":"                 { return token(COLON); }
"->"                { return token(ARROW); }
"."                 { return token(DOT); }
"#"                 { return token(SHARP); }
"="                 { return token(EQUAL); }

{INT}               { yylval->emplace<int>(atoi(yytext));
                      return token(INTEGER_LITERAL); }
{FLOAT}             { yylval->emplace<float>(atof(yytext));
                      return token(FLOAT_LITERAL); }
{STRING}            { string str = string(yytext+1, yyleng-2);
                      yylval->emplace<string>(str);
                      return token(STRING_LITERAL); }

{ID}                { yylval->emplace<string>(string(yytext));
                      return token(ID); }

"//".*              { }
"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>.          { }
<COMMENT>\n         { line_number++; position = 0; }

[ \t]+              { position += yyleng; }
\n                  { line_number++; position = 0; }


.                   { scanner_error("Illegal character: '"+string(yytext)+"'"); }

%%

int yyFlexLexer::yylex () { return 0; }

// End of work: spl.lex - 09/18/2025 - Nabih
