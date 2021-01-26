#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

extern int yylex();
extern int yyset_in();

void die_with_usage();
void lex_file(FILE *file);

#endif
