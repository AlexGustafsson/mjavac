#include "cfg.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#ifdef GRAPHVIZ_SUPPORT
#include <graphviz/gvc.h>
#endif

#include <mjavac/ast/nodes.hpp>
#include <mjavac/parser.hpp>
#include <mjavac/virtual-machine.hpp>

#include "ir-generator.hpp"
#include "program.hpp"
#include "semantics-analyzer.hpp"
#include "stdlib.hpp"
#include "symbol-generator.hpp"
#include "symbol-table.hpp"

#include "main.hpp"

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
  std::cout << std::setw(30) << std::left << "-h, --help" << std::setw(0) << " Print this help page" << std::endl;
  std::cout << std::setw(30) << std::left << "--parse-only" << std::setw(0) << " Only parse the source" << std::endl;
  std::cout << std::setw(30) << std::left << "--semantics-only" << std::setw(0) << " Only validate the semantics of the source" << std::endl;
  std::cout << std::setw(30) << std::left << "--execute" << std::setw(0) << " Execute the source" << std::endl;
  std::cout << std::setw(30) << std::left << "--ast <file.dot>" << std::setw(0) << " Output a dot-formatted parse graph" << std::endl;
  std::cout << std::setw(30) << std::left << "--cfg <file.dot>" << std::setw(0) << " Output a dot-formatted control flow graph" << std::endl;
#ifdef GRAPHVIZ_SUPPORT
  std::cout << std::setw(30) << std::left << "--ast-graph <file.(pdf|png|jpg)>" << std::setw(0) << " Render the parse graph as a pdf" << std::endl;
  std::cout << std::setw(30) << std::left << "--cfg-graph <file.(pdf|png|jpg)>" << std::setw(0) << " Render the control flow graph as a pdf" << std::endl;
#endif
  std::cout << std::setw(30) << std::left << "--symbol-table <file.txt>" << std::setw(0) << " Output the symbol table" << std::endl;
  std::cout << std::setw(30) << std::left << "--bytecode     <file.txt>" << std::setw(0) << " Output the bytecode" << std::endl;

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

  mjavac::ast::ProgramNode *program_node = nullptr;
  bool recovered_failure = false;
  mjavac::Scanner *scanner = new mjavac::Scanner(&input, source_path);
  mjavac::Parser *parser = new mjavac::Parser(*scanner, &program_node, &recovered_failure);

  bool parser_succeeded = parser->parse() == 0;
  input.close();

  if (!parser_succeeded || recovered_failure || program_node == nullptr)
    exit(EXIT_FAILURE);

  // Mount the system stdlib
  mount_stdlib_system(program_node);

  char *ast_dot_path = parameter(argc, argv, "--ast");
  bool generate_parse_graph = ast_dot_path != nullptr;
#ifdef GRAPHVIZ_SUPPORT
  char *ast_graph_path = parameter(argc, argv, "--ast-graph");
  generate_parse_graph |= ast_graph_path != nullptr;
#endif
  if (generate_parse_graph) {
    std::stringstream raw_dot_stream;
    program_node->generate_parse_graph(raw_dot_stream);
    std::string graph_string = raw_dot_stream.str();

    if (ast_dot_path != nullptr)
      write_dot(ast_dot_path, graph_string);

#ifdef GRAPHVIZ_SUPPORT
    char *ast_graph_path = parameter(argc, argv, "--ast-graph");
    if (ast_graph_path != nullptr)
      render_graph(ast_graph_path, graph_string);
#endif
  }

  if (flag_is_set(argc, argv, "--parse-only"))
    exit(EXIT_SUCCESS);

  SymbolTable *symbol_table = new SymbolTable();
  generate_symbols_for_program(symbol_table, program_node);

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

  bool semantics_analysis_succeeded = analyze_program_semantics(symbol_table, program_node);
  if (!semantics_analysis_succeeded)
    exit(EXIT_FAILURE);

  if (flag_is_set(argc, argv, "--semantics-only"))
    exit(EXIT_SUCCESS);

  Program *program = new Program(program_node);

  char *cfg_dot_path = parameter(argc, argv, "--cfg");
  bool generate_cfg_path = cfg_dot_path != nullptr;
#ifdef GRAPHVIZ_SUPPORT
  char *cfg_graph_path = parameter(argc, argv, "--cfg-graph");
  generate_cfg_path |= cfg_graph_path != nullptr;
#endif
  if (generate_cfg_path) {
    std::stringstream raw_dot_stream;
    program->write_cfg(raw_dot_stream);
    std::string graph_string = raw_dot_stream.str();

    if (cfg_dot_path != nullptr)
      write_dot(cfg_dot_path, graph_string);

#ifdef GRAPHVIZ_SUPPORT
    if (cfg_graph_path != nullptr)
      render_graph(cfg_graph_path, graph_string);
#endif
  }

  char *bytecode_path = parameter(argc, argv, "--bytecode");
  if (bytecode_path != nullptr) {
    std::ofstream bytecode_stream;
    bytecode_stream.open(bytecode_path);
    if (!bytecode_stream.is_open()) {
      std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << bytecode_path << ": " << strerror(errno) << std::endl;
      std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m unable to create output bytecode path file" << std::endl;
      exit(EXIT_FAILURE);
    }

    program->write_bytecode(bytecode_stream);
    bytecode_stream.close();
  }

  if (flag_is_set(argc, argv, "--execute")) {
    program->execute();
  }

  return 0;
}

void write_dot(std::string dot_path, std::string graph_string) {
  std::ofstream dot_stream;
  dot_stream.open(dot_path);
  if (!dot_stream.is_open()) {
    std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << dot_path << ": " << strerror(errno) << std::endl;
    std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m unable to create output dot file" << std::endl;
    exit(EXIT_FAILURE);
  }
  dot_stream << graph_string;
  dot_stream.close();
}

#ifdef GRAPHVIZ_SUPPORT
void render_graph(std::string graph_path, std::string graph_string) {
  std::filesystem::path parsed_graph_path(graph_path);
  std::string extension = parsed_graph_path.extension();

  if (extension.compare(".pdf") != 0 && extension.compare(".png") != 0 && extension.compare(".jpg") != 0) {
    std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m unsupported graph format '" << extension << "'. Expected one of '.pdf', '.png', '.jpg'" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ofstream graph_stream;
  graph_stream.open(graph_path);
  if (!graph_stream.is_open()) {
    std::cerr << "\033[1mmjavac: \033[31merror:\033[0m " << graph_path << ": " << strerror(errno) << std::endl;
    std::cerr << "\033[1mmjavac: \033[31mfatal error:\033[0m unable to create output graph file" << std::endl;
    exit(EXIT_FAILURE);
  }

  GVC_t *context = gvContext();
  Agraph_t *graph = agmemread(graph_string.c_str());
  gvLayout(context, graph, "dot");
  char *graph_data = nullptr;
  unsigned int graph_data_length = 0;
  gvRenderData(context, graph, extension.substr(1).c_str(), &graph_data, &graph_data_length);
  gvFreeLayout(context, graph);
  agclose(graph);
  gvFreeContext(context);

  graph_stream.write(graph_data, graph_data_length);
  graph_stream.close();
  gvFreeRenderData(graph_data);
}

#endif
