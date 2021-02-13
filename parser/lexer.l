%{
  #include "scanner.hpp"

  #undef  YY_DECL
  #define YY_DECL int mjavac::Scanner::yylex(mjavac::Parser::semantic_type * const lval, mjavac::Parser::location_type *location)

  #define YY_USER_ACTION location->step(); location->columns(yyleng);

  using namespace mjavac;
%}

%option yyclass="mjavac:Scanner"
%option noyywrap nounput batch noinput stack nodefault c++

%x COMMENTED_LINE

%%
%{
  yylval = lval;
%}

class {return Parser::token::KEYWORD_CLASS;}
private {return Parser::token::KEYWORD_PRIVATE;}
public {return Parser::token::KEYWORD_PUBLIC;}
static {return Parser::token::KEYWORD_STATIC;}
extends {return Parser::token::KEYWORD_EXTENDS;}
if {return Parser::token::KEYWORD_IF;}
else {return Parser::token::KEYWORD_ELSE;}
while {return Parser::token::KEYWORD_WHILE;}
new {return Parser::token::KEYWORD_NEW;}
return {return Parser::token::KEYWORD_RETURN;}
"("|")"|"["|"]"|"{"|"}"|";"|","|"="|"." {return yytext[0];}

[0-9]+ {
  yylval->build<std::string>(yytext);
  return Parser::token::INTEGER;
}
true|false {
  yylval->build<std::string>(yytext);
  return Parser::token::BOOLEAN;
}

int|boolean|void|String {
  yylval->build<std::string>(yytext);
  return Parser::token::TYPE;
}

"!" {return Parser::token::OPERATOR_NOT;}
"&&" {return Parser::token::OPERATOR_AND;}
"<" {return Parser::token::OPERATOR_LESS_THAN;}
"<=" {return Parser::token::OPERATOR_LESS_THAN_OR_EQUAL;}
">" {return Parser::token::OPERATOR_GREATER_THAN;}
">=" {return Parser::token::OPERATOR_GREATER_THAN_OR_EQUAL;}
"+" {return Parser::token::OPERATOR_PLUS;}
"-" {return Parser::token::OPERATOR_MINUS;}
"*" {return Parser::token::OPERATOR_MULTIPLICATION;}

[a-zA-Z_][a-zA-Z_0-9]* {
  yylval->build<std::string>(yytext);
  return Parser::token::IDENTIFIER;
}

"//" yy_push_state(COMMENTED_LINE);
<COMMENTED_LINE>[^\n]* /* ignore */
<COMMENTED_LINE>\n {location->lines(); yy_pop_state();}

[ \t]+ /* ignore */
\r?\n {location->lines();}

<<EOF>> {return Parser::token::END;};

. {std::cerr << "Failed to match: " << yytext << std::endl; return Parser::token::END;}
%%
