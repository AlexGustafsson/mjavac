#include <iostream>

#include "../debug.hpp"

#include "../semantics-analyzer.hpp"

bool analyze_statement_semantics(SymbolTableView *view, const ProgramNode *program_node, const MethodDeclarationNode *method_node, const Node *statement_node) {
  // Analyze conditionals (conditional)
  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    return true;
  }

  // Analyze loops (loop)
  const auto &loop_node = dynamic_cast<const LoopNode *>(statement_node);
  if (loop_node != nullptr) {
    return true;
  }

  // Analyze an assignment   (expression = expression)
  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr) {
    // Analyze a variable assignment
    if (binary_operation_node->binary_operator == Operator::Assign) {
      bool passed = true;

      // Analyze the variable declaration
      const auto &variable_node = dynamic_cast<const VariableNode *>(binary_operation_node->left);
      if (variable_node != nullptr) {
        // Error on array declaration of type other than int
        if (variable_node->type->is_array && variable_node->type->type.compare("int") != 0) {
          program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "array declaration of type other than int");
          passed = false;
        }

        // Error on duplicate variables
        if (view->count_symbols_by_name(variable_node->identifier) > 1) {
          program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "duplicate variable declaration");
          passed = false;
        }
      }

      // Analyze the expression
      passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      //   passed &= analyze_expression_semantics(view, program_node, binary_operation_node->right);

      //   const auto &left = dynamic_cast<const ValueNode *>(binary_operation_node->left);
      //   if (left->type == ValueNode::Identifier) {
      //     Symbol *variable_symbol = view->get_symbol_by_name(left->identifier_value);
      //     if (variable_symbol == nullptr) {
      //       // Not defined in this scope
      //       program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "assignment to non-defined variable");
      //       passed = false;
      //     } else {
      //       debug_out << "warning: unhandled assign " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;
      //     }
      //   } else if (left->type == ValueNode::Object) {
      //     debug_out << "warning: unhandled assign using object " << std::setbase(16) << binary_operation_node->get_id() << std::setbase(10) << std::endl;
      //   } else {
      //     program_node->source->print_line_error(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "assignment to non-variable");
      //     passed = false;
      //   }
      return passed;
    } else {
      program_node->source->print_line_warning(std::cerr, binary_operation_node->location.start_line, binary_operation_node->location.start_column, "unused result from operation");
      return analyze_expression_semantics(view, program_node, binary_operation_node);
    }
  }

  // Analyze a variable declaration (type identifier)
  const auto &variable_node = dynamic_cast<const VariableNode *>(statement_node);
  if (variable_node != nullptr) {
    bool passed = true;

    // Error on array declaration of type other than int
    if (variable_node->type->is_array && variable_node->type->type.compare("int") != 0) {
      program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "array declaration of type other than int");
      passed = false;
    }

    // Error on duplicate variables
    if (view->count_symbols_by_name(variable_node->identifier) > 1) {
      program_node->source->print_line_error(std::cerr, variable_node->location.start_line, variable_node->location.start_column, "duplicate variable declaration");
      passed = false;
    }

    return passed;
  }

  // Analyze a return
  const auto &return_node = dynamic_cast<const ReturnNode *>(statement_node);
  if (return_node != nullptr) {
    bool passed = true;

    if (return_node->value == nullptr) {
      if (method_node->type->type.compare("void") != 0) {
        program_node->source->print_line_error(std::cerr, return_node->location.start_line, return_node->location.start_column, "must return a value in non-void method");
        passed = false;
      }
    } else {
      if (method_node->type->type.compare("void") == 0) {
        program_node->source->print_line_error(std::cerr, return_node->location.start_line, return_node->location.start_column, "must not return a value in void method");
        passed = false;
      } else {
        Symbol *value = view->symbol_table->get_symbol(return_node->value->get_id());
        if (value == nullptr) {
          debug_out << "warning: missing symbol for return value " << std::setbase(16) << return_node->value->get_id() << std::setbase(10) << " for return " << std::setbase(16) << return_node->get_id() << std::setbase(10) << std::endl;
        } else {
          int flags = view->resolve_flags(value);
          bool is_valid_int = method_node->type->type.compare("int") == 0 && flags & SymbolTrait::IntLike;
          bool is_valid_boolean = method_node->type->type.compare("boolean") == 0 && flags & SymbolTrait::BooleanLike;
          if (!is_valid_int && !is_valid_boolean) {
            // TODO: Deactivate for now.
            // The handling of objects does not really work. The idea was to handle some recursive lookup via
            // "behaves like" which then resolves the value within the scope. This is not properly implemented
            // for "dot" operations - object accessors such as "this.works". Furthermore,
            // "this" doesn't work at all currently. How should it be handled? Right now the closest would be
            // to use the recursive lookup and when "this" is seen, use some local scope? Another way would
            // be to put it on the parser with a new "this accessors"...
            // Or, YAGNI and just hard-code stuff for library methods :(
            // program_node->source->print_line_error(std::cerr, return_node->value->location.start_line, return_node->value->location.start_column, "incorrect return type");
            // passed = false;
          }
        }

        passed &= analyze_expression_semantics(view, program_node, return_node->value);
      }
    }

    return passed;
  }

  // Analyze a basic expression
  return analyze_expression_semantics(view, program_node, statement_node);
}
