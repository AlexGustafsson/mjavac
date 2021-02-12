#include <fstream>
#include <iostream>

#include "parser.tab.hpp"
#include "scanner.hpp"

#include "main.hpp"

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("Running in debug mode\n");
#endif
  std::ifstream input;
  input.open(argv[1]);
  if (!input.is_open()) {
    std::cout << "Could not open file: " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }

  nodes::ProgramNode *program = nullptr;
  mjavac::Scanner *scanner = new mjavac::Scanner(&input, argv[1]);
  mjavac::Parser *parser = new mjavac::Parser(*scanner, &program);

  bool success = parser->parse() == 0;

  if (!success) {
    std::cerr << "Parsing failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ofstream diagram_stream;
  diagram_stream.open("tree.dot");
  program->generate_parse_graph(diagram_stream);
  diagram_stream.close();

  //Build symbol table
  //ST st;
  //root->buildST(st);

  //Semantic analysis
  //root->checkSemantics(st);

  return 0;
}
