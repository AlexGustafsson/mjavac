#include <fstream>
#include <memory>

#include "parser.tab.hpp"

#include "nodes/nodes.hpp"

#include "main.hpp"

int error_count = 0;
extern std::shared_ptr<nodes::ProgramNode> root;
std::ifstream inputFile;
std::string parsed = "";

void yy::parser::error(std::string const &err) {
  std::cout << "Parsing error: " << err << std::endl;
  std::cout << "<" << parsed << ">" << std::endl;
  exit(EXIT_FAILURE);
}

// Implementation to use for lexer's YY_INPUT macro
int input(char *buffer, int length) {
  int i;
  for (i = 0; i < length && inputFile.get(buffer[i]); i++)
    parsed += buffer[i];
  return i;
}

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("Running in debug mode\n");
#endif
  std::ofstream diagram_stream;
  diagram_stream.open("tree.dot");

  inputFile.open(argv[1]);
  if (!inputFile.is_open()) {
    std::cout << "Could not open file: " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }

  yy::parser parser;
  if (!parser.parse()) {
    if (root == nullptr) {
      std::cout << "Empty file" << std::endl;
      return 0;
    }

    diagram_stream << "digraph {" << std::endl;
    root->generate_parse_graph(diagram_stream);
    diagram_stream << "}" << std::endl;
    diagram_stream.close();

    //Build symbol table
    //ST st;
    //root->buildST(st);

    //Semantic analysis
    //root->checkSemantics(st);
  }

  return 0;
}
