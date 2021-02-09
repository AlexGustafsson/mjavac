#include <fstream>
#include <memory>

#include "parser.tab.hpp"

#include "node.hpp"

#include "main.hpp"

int error_count = 0;
extern std::shared_ptr<Node> root;

int main(int argc, char **argv) {
#ifdef DEBUG
  printf("Running in debug mode\n");
#endif
  std::ofstream diagram_stream;
  diagram_stream.open("tree.dot");

  yy::parser parser;
  if (!parser.parse()) {
    std::cout << "Built a parse-tree:" << std::endl;
    root->print_tree(0);

    diagram_stream << "digraph {" << std::endl;
    int count = 0;
    root->generate_tree(count, &diagram_stream);
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
