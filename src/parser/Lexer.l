%{
#include "ParseObject.h"
#include "Parser.hpp"

#define yylval uetli_parser_lval
#define SET_TOKEN(t) (yylval.token = t)
#define SET_STRING (yylval.string = new std::string(yytext, yyleng))

extern "C" int yywrap()
{
    return 1; /* do not continue on EOF */
}

%}


%option prefix="uetli_parser_"

%%



[\t ]                   ; // Space or tab ignored
\/\*[.\n]*\*\/          ; // comment
\n                      return SET_TOKEN(NEW_LINE);

":"                     return SET_TOKEN(COLON);
","                     return SET_TOKEN(COMMA);
":="                    return SET_TOKEN(ASSIGN);

"+"		             	return SET_TOKEN(PLUS);
"-"		             	return SET_TOKEN(MINUS);
"*"		             	return SET_TOKEN(ASTERISK);
"/"		             	return SET_TOKEN(SLASH);

"("                     return SET_TOKEN(ROUND_LEFT);
")"                     return SET_TOKEN(ROUND_RIGHT);

"class"                 return SET_TOKEN(CLASS);
"do"                    return SET_TOKEN(DO);
"end"                   return SET_TOKEN(END);


[a-zA-Z_][a-zA-Z0-9_]*  SET_STRING; return IDENTIFIER;
.                       printf("Unknown token!\n"); yyterminate();

%%

