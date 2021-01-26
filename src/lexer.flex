%top {
  #define YYSTYPE int
}

%option noyywrap

%x STRING

%%
[^"]*           { printf("OTHER: %s\n",yytext);}
["]              {BEGIN STRING;}
<STRING>["]      {BEGIN INITIAL;}
<STRING>[^"]*   {printf("STRING: %s\n", yytext);}
