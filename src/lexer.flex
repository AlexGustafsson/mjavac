%top {
  #define YYSTYPE int
}

%option noyywrap

quote_double \"

%x STRING_DOUBLE_QUOTED

%%
{quote_double} BEGIN(STRING_DOUBLE_QUOTED);
<STRING_DOUBLE_QUOTED>[^\"]* {printf("STRING_DOUBLE_QUOTED: %s\n", yytext);}
<STRING_DOUBLE_QUOTED>{quote_double} BEGIN(INITIAL);

<<EOF>> {printf("EOF\n");}
