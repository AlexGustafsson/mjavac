%top {
  #define YYSTYPE int
  extern int error_count;
}

%option noyywrap stack nodefault

whitespace [ \t]+
newline \n

keyword class|public|static|main|extends|if|while|"("|")"|"["|"]"|"{"|"}"|";"|","|"."|this|new|=
type int|boolean|void
function "System.out.println"
operator !|&&|<|"+"|-|"*"
value true|false|[0-9]+

comment_single_line "//"

identifier [a-zA-Z_][a-zA-Z0-9]*

%x COMMENTED_LINE

%%
{keyword} {printf("[Keyword '%s']", yytext);}
{type} {printf("[Type '%s']", yytext);}
{identifier} {printf("[Identifier '%s']", yytext);}
{function} {printf("[Function '%s']", yytext);}
{operator} {printf("[Operator '%s'", yytext);}
{value} {printf("[Value '%s']", yytext);}

{comment_single_line}{whitespace}* yy_push_state(COMMENTED_LINE);
<COMMENTED_LINE>[^\n]* {printf("[Comment '%s']\n", yytext);}
<COMMENTED_LINE>{newline} yy_pop_state();

{whitespace} /* ignore */
{newline}+ {printf("\n");}

. {printf("\033[31m[Unexpected '%s']\033[0m", yytext); error_count++;}
