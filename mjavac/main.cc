#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>

#include <mjavac/parser.hpp>

#include "main.hpp"
#include "symbol-table.hpp"

char *parameter(int argc, char **argv, const std::string &name) {
  char **begin = argv;
  char **end = begin + argc;
  char **iterator = std::find(begin, end, name);
  if (iterator != end && ++iterator != end)
    return *iterator;
  return 0;
}

bool flag_is_set(int argc, char **argv, const std::string &name) {
  char **begin = argv;
  char **end = begin + argc;
  return std::find(begin, end, name) != end;
}

void exit_with_usage(int code) {
  std::cout << "\033[1mOVERVIEW\033[0m:  MiniJava compiler with extensions" << std::endl;
  std::cout << std::endl;
  std::cout << "\033[1mUSAGE\033[0m: mjavac [options] file" << std::endl;
  std::cout << std::endl;
  std::cout << "\033[1mOPTIONS\033[0m:" << std::endl;
  std::cout << std::endl;
  std::cout << std::setw(15) << "-h, --help" << std::setw(0) << " Print this help page" << std::endl;
  std::cout << std::setw(15) << "--dot" << std::setw(0) << " Output a dot-formatted parse graph" << std::endl;
  std::cout << std::setw(15) << "--symbol-table" << std::setw(0) << " Output the symbol table" << std::endl;
  std::cout << std::setw(15) << "--parse-only" << std::setw(0) << " Only parse the source" << std::endl;
  std::cout << std::setw(15) << "--semantics-only" << std::setw(0) << " Only validate the semantics of the source" << std::endl;
  std::cout << std::setw(15) << "--execute" << std::setw(0) << " Execute the source" << std::endl;

  exit(code);
}

int main(int argc, char **argv) {
  if (argc <= 1)
    exit_with_usage(EXIT_FAILURE);

  if (flag_is_set(argc, argv, "-h") || flag_is_set(argc, argv, "--help"))
    exit_with_usage(EXIT_SUCCESS);

  char *source_path = argv[argc - 1];
  std::ifstream input;
  input.open(source_path);
  if (!input.is_open()) {
    std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << source_path << ": " << strerror(errno) << std::endl;
    std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m no input files" << std::endl;
    std::cerr << "compilation terminated." << std::endl;
    exit(EXIT_FAILURE);
  }

  mjavac::nodes::ProgramNode *program = nullptr;
  mjavac::Scanner *scanner = new mjavac::Scanner(&input, source_path);
  mjavac::Parser *parser = new mjavac::Parser(*scanner, &program);

  bool success = parser->parse() == 0;
  input.close();

  if (!success)
    exit(EXIT_FAILURE);

  char *dot_path = parameter(argc, argv, "--dot");
  if (dot_path != nullptr) {
    std::ofstream dot_stream;
    dot_stream.open(dot_path);
    if (!dot_stream.is_open()) {
      std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << dot_path << ": " << strerror(errno) << std::endl;
      std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m unable to create output dot file" << std::endl;
      exit(EXIT_FAILURE);
    }

    program->generate_parse_graph(dot_stream);
    dot_stream.close();

#ifdef GRAPHVIZ_SUPPORT
    char *graph_path = parameter(argc, argv, "--graph");
#endif
  }

  if (flag_is_set(argc, argv, "--parse-only"))
    exit(EXIT_SUCCESS);

  SymbolTable *symbol_table = new SymbolTable();

  // Add the program's symbol
  symbol_table->symbols[program->get_id()] = new Symbol(program, program->get_id(), SymbolTrait::None);

  for (const auto& classNode : program->declarations) {
    // Add a symbol for the class
    symbol_table->symbols[classNode->get_id()] = new Symbol(classNode, program->get_id(), SymbolTrait::Accessible | SymbolTrait::Initializable);
  }

  char *symbol_table_path = parameter(argc, argv, "--symbol-table");
  if (symbol_table_path != nullptr) {
    std::ofstream symbol_table_stream;
    symbol_table_stream.open(symbol_table_path);
    if (!symbol_table_stream.is_open()) {
      std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << symbol_table_path << ": " << strerror(errno) << std::endl;
      std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m unable to create output symbol path file" << std::endl;
      exit(EXIT_FAILURE);
    }

    symbol_table->write(symbol_table_stream);
    symbol_table_stream.close();
  }

  if (flag_is_set(argc, argv, "--semantics-only"))
    exit(EXIT_SUCCESS);

  if (flag_is_set(argc, argv, "--execute")) {
    std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m execution not implemented" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Traverse the tree and create a symbol tree
  // node id -> symbol traits
  // traits: int-like, callable, string-like, subscriptable, etc.
  // if (symbol_table[node_id] != callable) error(not callable)

  return 0;
}
