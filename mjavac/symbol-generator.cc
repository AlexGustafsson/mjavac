#include <iostream>

#include "debug.hpp"

#include "mjavac/ast/array-initialization-node.hpp"
#include "mjavac/ast/class-initialization-node.hpp"
#include "mjavac/ast/method-call-node.hpp"
#include "symbol-generator.hpp"

void generate_symbols_for_program(SymbolTable *symbol_table, const ProgramNode *program_node) {
  // Add the program's symbol
  symbol_table->add_symbol(new Symbol(program_node, 0, SymbolTrait::None));

  // Add each class
  // There is some nasty bug here. If there are no declarations in the program (empty source)
  // then declarations will have zero elements, but .empty() returns false. Therefore
  // the loop below will still run once, yielding a nullptr for the class node.
  // According to C++ documentation, the range-based for loop should handle that case
  // but apparently it doesn't
  // Using built - in specs.COLLECT_GCC = gcc
  // COLLECT_LTO_WRAPPER = / usr / local / Cellar / gcc / 10.2.0_3 / libexec / gcc / x86_64 - apple - darwin20 / 10.2.0 / lto - wrapper Target : x86_64 - apple - darwin20 Configured with :../ configure-- build = x86_64 - apple - darwin20-- prefix = / usr / local / Cellar / gcc / 10.2.0_3 --libdir = / usr / local / Cellar / gcc / 10.2.0_3 / lib / gcc / 10 --disable - nls-- enable - checking = release-- enable - languages = c,
  // c++, objc, obj - c++, fortran-- program - suffix = -10 --with - gmp = / usr / local / opt / gmp-- with - mpfr = / usr / local / opt / mpfr-- with - mpc = / usr / local / opt / libmpc-- with - isl = / usr / local / opt / isl-- with - system - zlib-- with - pkgversion = 'Homebrew GCC 10.2.0_3' --with - bugurl = https : //github.com/Homebrew/homebrew-core/issues --disable-multilib --with-native-system-header-dir=/usr/include --with-sysroot=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk SED=/usr/bin/sed
  // Thread model : posix Supported LTO compression algorithms : zlib gcc version 10.2.0(Homebrew GCC 10.2.0_3)
  // This check works around that issue.
  if (program_node->declarations.size() == 0)
    return;
  for (const auto &class_node : program_node->declarations) {
    generate_symbols_for_class(symbol_table, class_node, program_node);
  }
}

void generate_symbols_for_class(SymbolTable *symbol_table, const ClassDeclarationNode *class_node, const ProgramNode *program_node) {
  // Add a symbol for the class
  symbol_table->add_symbol(new Symbol(class_node, class_node->identifier, program_node->get_id(), SymbolTrait::Accessible | SymbolTrait::Initializable));

  // Add symbols for variables
  for (const auto &variable_node : class_node->variable_declarations)
    generate_symbols_for_variable(symbol_table, variable_node, class_node);

  // Add symbols for variable assignments
  for (const auto &variable_assignment : class_node->variable_assignments) {
    const VariableNode *variable_node = dynamic_cast<const VariableNode *>(variable_assignment);
    if (variable_node == nullptr)
      continue; // TODO: Print error
    generate_symbols_for_variable(symbol_table, variable_node, class_node);
  }

  // Add symbols for methods
  for (const auto &method_node : class_node->method_declarations) {
    // Add a symbol for the method itself
    symbol_table->add_symbol(new Symbol(method_node, method_node->identifier, class_node->get_id(), SymbolTrait::Callable));
  }

  // Recurse after defining the "prototypes" to make them known in the scope
  for (const auto &method_node : class_node->method_declarations) {
    generate_symbols_for_method(symbol_table, method_node, class_node);
  }
}

void generate_symbols_for_variable(SymbolTable *symbol_table, const VariableNode *variable_node, const Node *scope_node) {
  int traits = SymbolTrait::None;
  if (variable_node->type->is_array)
    traits |= SymbolTrait::Subscriptable;
  if (variable_node->type->type == "int")
    traits |= SymbolTrait::IntLike;
  if (variable_node->type->type == "boolean")
    traits |= SymbolTrait::BooleanLike;
  symbol_table->add_symbol(new Symbol(variable_node, variable_node->identifier, scope_node->get_id(), traits));
}

void generate_symbols_for_method(SymbolTable *symbol_table, const MethodDeclarationNode *method_node, const ClassDeclarationNode *class_node) {
  // Add parameters
  for (const auto &parameter_node : method_node->parameters)
    generate_symbols_for_variable(symbol_table, parameter_node, method_node);

  // Add statements
  for (const auto &statement_node : method_node->statements)
    generate_symbols_for_statement(symbol_table, statement_node, method_node);
}

void generate_symbols_for_statement(SymbolTable *symbol_table, const Node *statement_node, const Node *scope_node) {
  const auto &loop_node = dynamic_cast<const LoopNode *>(statement_node);
  if (loop_node != nullptr) {
    // Generate a symbol (and scope) for the condition
    symbol_table->add_symbol(new Symbol(loop_node, scope_node->get_id(), SymbolTrait::None));
    // Let the expression be part of the same scope as the loop itself
    generate_symbols_for_expression(symbol_table, loop_node->expression, loop_node);
    // Add all statements to the loop's scope
    for (const auto &loope_statement_node : loop_node->statements)
      generate_symbols_for_statement(symbol_table, loope_statement_node, loop_node);
    return;
  }

  const auto &conditional_node = dynamic_cast<const ConditionalNode *>(statement_node);
  if (conditional_node != nullptr) {
    // Generate a symbol (and scope) for the condition
    symbol_table->add_symbol(new Symbol(conditional_node, scope_node->get_id(), SymbolTrait::None));
    // Let the expression be part of the same scope as the condition itself
    generate_symbols_for_expression(symbol_table, conditional_node->expression, scope_node);
    // Add all statements to the condition's scope
    for (const auto &nested_statement_node : conditional_node->statements)
      generate_symbols_for_statement(symbol_table, nested_statement_node, conditional_node);
    // For now, only "else" is implemented, meaning there's no need to loop over chained if statements
    if (conditional_node->next != nullptr) {
      for (const auto &nested_statement_node : conditional_node->next->statements)
        generate_symbols_for_statement(symbol_table, nested_statement_node, conditional_node);
    }
    return;
  }

  const auto &variable_node = dynamic_cast<const VariableNode *>(statement_node);
  if (variable_node != nullptr) {
    generate_symbols_for_variable(symbol_table, variable_node, scope_node);
    return;
  }

  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(statement_node);
  if (binary_operation_node != nullptr && binary_operation_node->binary_operator == Operator::Assign) {
    // If the assign is for a new declaration, handle it appropriately
    // otherwise, handle it like any other assignment
    const auto &variable_node = dynamic_cast<const VariableNode *>(binary_operation_node->left);
    if (variable_node == nullptr)
      generate_symbols_for_expression(symbol_table, binary_operation_node->left, scope_node);
    else
      generate_symbols_for_variable(symbol_table, variable_node, scope_node);
    generate_symbols_for_expression(symbol_table, binary_operation_node->right, scope_node);
    return;
  }

  const auto &return_node = dynamic_cast<const ReturnNode *>(statement_node);
  if (return_node != nullptr) {
    if (return_node->value != nullptr)
      generate_symbols_for_expression(symbol_table, return_node->value, scope_node);
    return;
  }

  generate_symbols_for_expression(symbol_table, statement_node, scope_node);
}

void generate_symbols_for_expression(SymbolTable *symbol_table, const Node *expression_node, const Node *scope_node) {
  const auto &value_node = dynamic_cast<const ValueNode *>(expression_node);
  if (value_node != nullptr) {
    if (value_node->type == ValueNode::Integer)
      symbol_table->add_symbol(new Symbol(value_node, scope_node->get_id(), SymbolTrait::IntLike));
    else if (value_node->type == ValueNode::Boolean)
      symbol_table->add_symbol(new Symbol(value_node, scope_node->get_id(), SymbolTrait::BooleanLike));
    else if (value_node->type == ValueNode::Identifier)
      symbol_table->add_symbol(new Symbol(value_node, scope_node->get_id(), SymbolTrait::BehavesLikeIdentifier, value_node->identifier_value));
    else
      debug_out << "warning: unhandled value type " << value_node->type << std::endl;
    // TODO: add further types
    return;
  }

  const auto &binary_operation_node = dynamic_cast<const BinaryOperationNode *>(expression_node);
  if (binary_operation_node != nullptr) {
    generate_symbols_for_expression(symbol_table, binary_operation_node->left, scope_node);
    generate_symbols_for_expression(symbol_table, binary_operation_node->right, scope_node);

    if (binary_operation_node->binary_operator == Operator::And || binary_operation_node->binary_operator == Operator::LessThan || binary_operation_node->binary_operator == Operator::LessThanOrEqual || binary_operation_node->binary_operator == Operator::GreaterThan || binary_operation_node->binary_operator == Operator::GreaterThanOrEqual || binary_operation_node->binary_operator == Operator::Equal || binary_operation_node->binary_operator == Operator::NotEqual || binary_operation_node->binary_operator == Operator::Or || binary_operation_node->binary_operator == Operator::Negate) {
      symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), SymbolTrait::BooleanLike));
    } else if (binary_operation_node->binary_operator == Operator::Plus || binary_operation_node->binary_operator == Operator::Minus || binary_operation_node->binary_operator == Operator::Multiplication || binary_operation_node->binary_operator == Operator::Division || binary_operation_node->binary_operator == Operator::Negative) {
      symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), SymbolTrait::IntLike));
    } else if (binary_operation_node->binary_operator == Operator::Dot) {
      // TODO: Add proper lookup
      symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), SymbolTrait::None));
      debug_out << "warning: unimplemented symbol generation for binary operation 'dot'" << std::endl;
    } else if (binary_operation_node->binary_operator == Operator::Subscript) {
      // At this stage, even though the language only supports int arrays, assume that an array subscript
      // has the type of the symbol it subscripts
      Symbol *left_symbol = symbol_table->get_symbol(binary_operation_node->left->get_id());
      if (left_symbol) {
        int traits = (left_symbol->traits & SymbolTrait::IntLike) | (left_symbol->traits & SymbolTrait::BooleanLike) | (left_symbol->traits & SymbolTrait::StringLike);
        symbol_table->add_symbol(new Symbol(binary_operation_node, scope_node->get_id(), traits));
      } else {
        debug_out << "warning: missing symbol for value" << binary_operation_node->left->get_id() << " in subscript " << binary_operation_node->get_id() << std::endl;
      }
    } else {
      debug_out << "warning: unhandled binary operator for expression: " << binary_operation_node->binary_operator << std::endl;
    }

    return;
  }

  const auto &unary_operation_node = dynamic_cast<const UnaryOperationNode *>(expression_node);
  if (unary_operation_node != nullptr) {
    generate_symbols_for_expression(symbol_table, unary_operation_node->operand, scope_node);
    if (unary_operation_node->unary_operator == Operator::Negate)
      symbol_table->add_symbol(new Symbol(unary_operation_node, scope_node->get_id(), SymbolTrait::BooleanLike));
    else if (unary_operation_node->unary_operator == Operator::Negative)
      symbol_table->add_symbol(new Symbol(unary_operation_node, scope_node->get_id(), SymbolTrait::IntLike));
    else
      debug_out << "warning: unhandled unary operator for expression: " << unary_operation_node->unary_operator << std::endl;
    return;
  }

  const auto &array_initialization_node = dynamic_cast<const ArrayInitializationNode *>(expression_node);
  if (array_initialization_node != nullptr) {
    generate_symbols_for_expression(symbol_table, array_initialization_node->initialization_expression, scope_node);
    int traits = SymbolTrait::Subscriptable;
    if (array_initialization_node->type->type == "int")
      traits |= SymbolTrait::IntLike;
    if (array_initialization_node->type->type == "boolean")
      traits |= SymbolTrait::BooleanLike;
    symbol_table->add_symbol(new Symbol(array_initialization_node, scope_node->get_id(), traits));
    return;
  }

  const auto &class_initialization_node = dynamic_cast<const ClassInitializationNode *>(expression_node);
  if (class_initialization_node != nullptr) {
    symbol_table->add_symbol(new Symbol(class_initialization_node, scope_node->get_id(), SymbolTrait::Accessible));
    return;
  }

  const auto &method_call = dynamic_cast<const MethodCallNode *>(expression_node);
  if (method_call != nullptr) {
    symbol_table->add_symbol(new Symbol(method_call, scope_node->get_id(), SymbolTrait::BehavesLikeNode, method_call->value));
    return;
  }

  debug_out << "warning: unhandled expression generation " << expression_node->get_id() << std::endl;
}
