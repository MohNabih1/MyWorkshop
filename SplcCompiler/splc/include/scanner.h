namespace splc {

class Scanner: public yyFlexLexer {
public:
    Scanner ( istream &yyin, ostream &yyout ): yyFlexLexer(yyin,yyout) {}
    Scanner ( istream* yyin = nullptr, ostream* yyout = nullptr): yyFlexLexer(yyin,yyout) {}
    int lex ( Parser::semantic_type* yylval );
};

}
