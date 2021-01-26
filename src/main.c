#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("Running in debug mode\n");
#endif

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      lex_file(file);
    }
  } else {
    lex_file(stdin);
  }

  return 0;
}

void lex_file(FILE *file) {
  if (file == NULL) {
    perror("Unable to open file");
    exit(1);
  }

  yyset_in(file);
  yylex();
  fclose(file);
}

void die_with_usage(char **argv) {
  printf("%s: [source file 1, source file 2, ...]\n", argv[0]);
  printf("If no file is specified, STDIN is used");
  exit(1);
}
