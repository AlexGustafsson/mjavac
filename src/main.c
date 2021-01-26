#include <stdio.h>

#include "main.h"

int yyerror(char *c) {
  printf("It's a bad: %s\n", c);
  return 0;
}

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("Running in debug mode\n");
#endif

  if (!yylex())
    printf("Done!\n");
  return 0;
}
