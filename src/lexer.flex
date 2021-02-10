%top {
  #include "parser.tab.hpp"
  #define YY_DECL yy::parser::symbol_type yylex()
  extern int error_count;

  extern int input(char* buffer, int length);

  #define YY_INPUT(buffer, result, length) (result = input(buffer, length));
}

%option noyywrap nounput batch noinput stack nodefault

%x COMMENTED_LINE

%%
class {return yy::parser::make_KEYWORD_CLASS();}
public {return yy::parser::make_KEYWORD_PUBLIC();}
static {return yy::parser::make_KEYWORD_STATIC();}
extends {return yy::parser::make_KEYWORD_EXTENDS();}
if {return yy::parser::make_KEYWORD_IF();}
else {return yy::parser::make_KEYWORD_ELSE();}
while {return yy::parser::make_KEYWORD_WHILE();}
"(" {return yy::parser::make_KEYWORD_PARANTHESES_LEFT();}
")" {return yy::parser::make_KEYWORD_PARANTHESES_RIGHT();}
"[" {return yy::parser::make_KEYWORD_BRACKET_LEFT();}
"]" {return yy::parser::make_KEYWORD_BRACKET_RIGHT();}
"{" {return yy::parser::make_KEYWORD_BRACE_LEFT();}
"}" {return yy::parser::make_KEYWORD_BRACE_RIGHT();}
";" {return yy::parser::make_KEYWORD_SEMICOLON();}
"," {return yy::parser::make_KEYWORD_COMMA();}
this {return yy::parser::make_KEYWORD_THIS();}
new {return yy::parser::make_KEYWORD_NEW();}
"=" {return yy::parser::make_KEYWORD_EQUAL();}
"." {return yy::parser::make_KEYWORD_DOT();}

[0-9]+ {return yy::parser::make_INTEGER(yytext);}
true|false {return yy::parser::make_BOOLEAN(yytext);}

int|boolean|void {return yy::parser::make_TYPE(yytext);}

"!" {return yy::parser::make_OPERATOR_NOT();}
"&&" {return yy::parser::make_OPERATOR_AND();}
"<" {return yy::parser::make_OPERATOR_LESS_THAN();}
"<=" {return yy::parser::make_OPERATOR_LESS_THAN_OR_EQUAL();}
">" {return yy::parser::make_OPERATOR_GREATER_THAN();}
">=" {return yy::parser::make_OPERATOR_GREATER_THAN_OR_EQUAL();}
"+" {return yy::parser::make_OPERATOR_PLUS();}
"-" {return yy::parser::make_OPERATOR_MINUS();}
"*" {return yy::parser::make_OPERATOR_MULTIPLICATION();}

[a-zA-Z_][a-zA-Z0-9]* {return yy::parser::make_IDENTIFIER(yytext);}

"//" yy_push_state(COMMENTED_LINE);
<COMMENTED_LINE>[^\n]* /* ignore */
<COMMENTED_LINE>\n yy_pop_state();

[ \t]+ /* ignore */
\r?\n+ /* ignore */

<<EOF>> {return yy::parser::make_END();};

. {printf("\033[31m[Unexpected '%s']\033[0m", yytext); error_count++;}
