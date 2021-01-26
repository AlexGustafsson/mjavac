#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int yyerror(char *c) {
  printf("It's a bad: %s\n", c);
  return 0;
}

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("Running in debug mode\n");
#endif

  if (argc == 1)
    die_with_usage(argv);

  for (int i = 1; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL) {
      perror("Unable to open file: ");
      exit(1);
    }

    yyset_in(file);
    yylex();
    fclose(file);
  }

  return 0;
}

void die_with_usage(char **argv) {
  printf("%s: <source file 1> [source file 2, source file 3, ...]\n", argv[0]);
  exit(1);
}
