#include "../stdlib.hpp"

void mount_stdlib_system(ProgramNode *program_node) {
  // Create a class definition node for the System class
  ClassDeclarationNode *system_class_node = new ClassDeclarationNode("System");
  system_class_node->internal = true;
  program_node->declarations.push_back(system_class_node);

  ClassDeclarationNode *stream_class_node = new ClassDeclarationNode("Stream");
  stream_class_node->internal = true;
  program_node->declarations.push_back(stream_class_node);

  // Create a variable definition for the 'out' member
  VariableNode *variable_node = new VariableNode(new TypeNode("Stream"), "out");
  system_class_node->variable_declarations.push_back(variable_node);

  // Create a method for the 'println' method
  MethodDeclarationNode *println_method_node = new MethodDeclarationNode(true, false);
  println_method_node->type = new TypeNode("void");
  println_method_node->identifier = "println";
  VariableNode *parameter_node = new VariableNode(new TypeNode("int"), "output");
  println_method_node->parameters.push_back(parameter_node);
  stream_class_node->method_declarations.push_back(println_method_node);
}
