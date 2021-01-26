%top {
  #define YYSTYPE int
}

%option noyywrap stack

whitespace [ \t\n]+

quote_double \"
semicolon ;
comma ,

int int
boolean boolean
void void
string String

bracket_left "["
bracket_right "]"

parenthesis_left "("
parenthesis_right ")"

brace_left "{"
brace_right "}"

keyword_class class
keyword_public public
keyword_static static
keyword_main main
keyword_extends extends
keyword_if if

comment_single_line "//"

identifier [a-zA-Z_][a-zA-Z0-9]*

type boolean|int

%x COMMENTED_LINE
%x STATEMENT
%x CLASS_DECLERATION
%x EXPRESSION

%%
{keyword_class}{whitespace}{identifier}({whitespace}{keyword_extends}{whitespace}{identifier})?{whitespace}?{brace_left} {printf("<CLASS_DECLERATION>\n"); yy_push_state(CLASS_DECLERATION);}

<CLASS_DECLERATION>{type}{whitespace}{identifier}{semicolon} {printf("VAR_DECLERATION: %s\n", yytext);}

<CLASS_DECLERATION>{keyword_public}{whitespace}{keyword_static}{whitespace}{void}{whitespace}{keyword_main}{whitespace}?{parenthesis_left}{string}{bracket_left}{bracket_right}{whitespace}{identifier}{parenthesis_right}{whitespace}?{brace_left} {printf("<MAIN_STATEMENT>\n"); yy_push_state(STATEMENT);}
<CLASS_DECLERATION>{keyword_public}{whitespace}{type}{whitespace}{identifier}{whitespace}?{parenthesis_left}({type}{whitespace}{identifier}({whitespace}?{comma}{whitespace}?{type}{whitespace}{identifier})*)?{parenthesis_right}{whitespace}?{bracket_left} {printf("<METHOD>\n"); yy_push_state(STATEMENT);}

<STATEMENT>{keyword_if}{whitespace}? {printf("IF_CASE\n"); yy_push_state_(EXPRESSION);}



<STATEMENT>{brace_right} {printf("</STATEMENT>\n"); yy_pop_state();}

<CLASS_DECLERATION>{brace_right} {printf("</CLASS_DECLERATION>\n"); yy_pop_state();};

<CLASS_DECLERATION,STATEMENT>{comment_single_line} yy_push_state(COMMENTED_LINE);
<COMMENTED_LINE>[^\n]* {printf("COMMENTED_LINE: %s\n", yytext);}
<COMMENTED_LINE>\n yy_pop_state();

<INITIAL,CLASS_DECLERATION,STATEMENT>{whitespace}+ /* noop */
