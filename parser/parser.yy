%skeleton "lalr1.cc"
%require "3.7"
%defines
%define api.namespace {mjavac}
%define parser_class_name {Parser}
%define parse.error verbose
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include <iomanip>

  #include "nodes/nodes.hpp"
  using namespace mjavac::nodes;

  // Forward declarations
  namespace mjavac {
    class Scanner;
  }
}

%parse-param { Scanner &scanner }
%parse-param { ProgramNode **root }

%code {
  #include "scanner.hpp"

  #undef yylex
  #define yylex scanner.yylex
}

%token KEYWORD_CLASS KEYWORD_PUBLIC KEYWORD_PRIVATE KEYWORD_STATIC KEYWORD_EXTENDS
%token KEYWORD_IF KEYWORD_WHILE KEYWORD_RETURN KEYWORD_NEW KEYWORD_ELSE

%token <std::string> INTEGER BOOLEAN TYPE IDENTIFIER

%token OPERATOR_NOT OPERATOR_AND OPERATOR_LESS_THAN OPERATOR_LESS_THAN_OR_EQUAL
%token OPERATOR_GREATER_THAN OPERATOR_GREATER_THAN_OR_EQUAL OPERATOR_PLUS
%token OPERATOR_MINUS OPERATOR_MULTIPLICATION

%token END 0 "end of file"

%type <ProgramNode*> Program

%type <std::list<ClassDeclarationNode*>> ClassDeclarations
%type <ClassDeclarationNode*> ClassDeclaration

%type <std::list<Node*>> Declarations
%type <Node*> Declaration

%type <VariableNode*> VariableDeclaration
%type <MethodDeclarationNode*> MethodDeclaration
%type <MethodDeclarationNode*> MethodScopeDeclaration

%type <std::list<VariableNode*>> MethodParameters

%type <std::list<Node*>> Statements Expressions
%type <Node*> Statement Expression Chainable

%type <LoopNode*> Loop

%type <ValueNode*> Value

%type <std::string> Operator

%type <MethodCallNode*> MethodCall

%type <std::list<std::string>> ObjectList
%type <std::string> Object

%type <std::list<Node*>> ParameterList
%type <Node*> Parameter

%locations
%start Program

%%

Program : ClassDeclarations { *root = new ProgramNode(scanner.file_name); (*root)->declarations = $1; }
  | END { *root = new ProgramNode(scanner.file_name); }
  ;

ClassDeclarations : ClassDeclaration { $$.push_back($1); }
  | ClassDeclarations ClassDeclaration { $$ = $1; $$.push_back($2); }
  ;

ClassDeclaration : KEYWORD_CLASS IDENTIFIER KEYWORD_EXTENDS IDENTIFIER '{' Declarations '}' { $$ = new ClassDeclarationNode($2, $4); $$->setDeclarations($6); }
  | KEYWORD_CLASS IDENTIFIER KEYWORD_EXTENDS IDENTIFIER '{' '}' { $$ = new ClassDeclarationNode($2, $4); }
  | KEYWORD_CLASS IDENTIFIER '{' Declarations '}' { $$ = new ClassDeclarationNode($2); $$->setDeclarations($4); }
  | KEYWORD_CLASS IDENTIFIER '{' '}' { $$ = new ClassDeclarationNode($2); }
  ;

Declarations : Declaration { $$.push_back($1); }
  | Declarations Declaration { $$ = $1; $$.push_back($2); }
  ;

Declaration : VariableDeclaration ';' { $$ = $1; }
  | MethodDeclaration { $$ = $1; }
  ;

VariableDeclaration : TYPE '[' ']' IDENTIFIER { $$ = new VariableNode($1, $4, true, true); }
  | TYPE IDENTIFIER { $$ = new VariableNode($1, $2, true); }
  ;

MethodDeclaration : MethodScopeDeclaration TYPE IDENTIFIER '(' MethodParameters ')' '{' Statements '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; $$->statements = $8; }
  | MethodScopeDeclaration TYPE IDENTIFIER '(' ')' '{' Statements '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->statements = $7; }
  | MethodScopeDeclaration TYPE IDENTIFIER '(' MethodParameters ')' '{' '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; }
  | MethodScopeDeclaration TYPE IDENTIFIER '(' ')' '{' '}' { $$ = $1; $$->type = $2; $$->identifier = $3; }
  ;

MethodScopeDeclaration : KEYWORD_PUBLIC KEYWORD_STATIC { $$ = new MethodDeclarationNode(true, true); }
  | KEYWORD_PRIVATE KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  | KEYWORD_PUBLIC { $$ = new MethodDeclarationNode(true, false); }
  | KEYWORD_PRIVATE { $$ = new MethodDeclarationNode(false, false); }
  | KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  ;

MethodParameters : VariableDeclaration { $$.push_back($1); }
  | MethodParameters ',' VariableDeclaration { $$ = $1; $1.push_back($3); }
  ;

Statements : Statement { $$.push_back($1); }
  | Statements Statement { $$ = $1; $$.push_back($2); }
  ;

Statement : KEYWORD_IF '(' Expression ')' Statement KEYWORD_ELSE Statement { $$ = new ConditionalNode($3, $5, $7); }
  | Loop { $$ = $1; }
  | Expression ';' { $$ = $1; }
  | VariableDeclaration '=' Expression ';' { $$ = $1; $1->assigned_value = $3; }
  | VariableDeclaration ';' { $$ = $1; }
  | IDENTIFIER '=' Expression ';' { VariableNode* declaration = new VariableNode("variable", $1, false); declaration->assigned_value=$3; $$ = declaration; }
  | KEYWORD_RETURN Expression ';' { $$ = new ReturnNode($2); }
  | KEYWORD_RETURN ';' { $$ = new ReturnNode(); }
  ;

Loop : KEYWORD_WHILE '(' Expression ')' '{' Statements '}' { $$ = new LoopNode($3); $$->statements = $6; }
  | KEYWORD_WHILE '(' Expression ')' Statement { $$ = new LoopNode($3); $$->statements.push_back($5); }
  ;

Expressions : Expression { $$.push_back($1); }
  | Expressions Expression { $$ = $1; $$.push_back($2); }
  ;

Expression : Expression Operator Expression { $$ = new BinaryOperationNode($1, $3, $2); }
  | '(' Expression ')' { $$ = $2; }
  | Chainable { $$ = $1; }
  | Chainable '.' MethodCall { $$ = new BinaryOperationNode($1, $3, "."); }
  | Value { $$ = $1; }
  | Value '[' Expression ']' { $$ = $1; $1->is_array = true; $1->array_index = $3; }
  | Chainable '[' Expression ']' { $$ = new BinaryOperationNode($1, $3, "[]"); }
  ;

Chainable : MethodCall { $$ = $1; }
  | KEYWORD_NEW MethodCall { $$ = $2; $2->is_new = true; }
  ;

Value : INTEGER { $$ = new ValueNode(ValueNode::Integer, $1); }
  | BOOLEAN { $$ = new ValueNode(ValueNode::Boolean, $1); }
  | IDENTIFIER { $$ = new ValueNode(ValueNode::Identifier, $1); }
  | ObjectList { $$ = new ValueNode(ValueNode::Object, $1); }
  ;

Operator : OPERATOR_AND { $$ = Operator::And; }
  | OPERATOR_LESS_THAN { $$ = Operator::LessThan; }
  | OPERATOR_LESS_THAN_OR_EQUAL { $$ = Operator::LessThanOrEqual; }
  | OPERATOR_GREATER_THAN { $$ = Operator::GreaterThan; }
  | OPERATOR_GREATER_THAN_OR_EQUAL { $$ = Operator::GreaterThanOrEqual; }
  | OPERATOR_PLUS { $$ = Operator::Plus; }
  | OPERATOR_MINUS { $$ = Operator::Minus; }
  | OPERATOR_MULTIPLICATION { $$ = Operator::Multiplication; }
  ;

MethodCall : Value '(' ParameterList ')' { $$ = new MethodCallNode(); $$->value = $1; $$->parameters = $3; }
  | Value '(' ')' { $$ = new MethodCallNode(); $$->value = $1; }
  ;

ObjectList : IDENTIFIER { $$.push_back($1); }
  | ObjectList '.' IDENTIFIER { $$ = $1; $$.push_back($3); }
  ;

ParameterList : Expression { $$.push_back($1); }
  | ParameterList ',' Expression { $$ = $1; $$.push_back($3); }
  ;

%%

void mjavac::Parser::error(const location_type &location, const std::string &error) {
  // NOTE: The "coordinates" given in location are one-based values.
  // The scanner, on the other hand, has zero-based values.

  std::cerr << "\033[1m" << scanner.file_name << ":" << location.begin.line << ":" << location.begin.column << "\033[0m error: " << error << std::endl;
  std::cerr << std::setw(5) << location.begin.line << std::setw(0) << " |   ";

  // Extract the relevant line from the scanner's buffer
  size_t previous_index = 0;
  size_t index = 0;
  // The current line being parsed
  std::string_view line;
  // The index of the first line in the buffer
  int line_index = scanner.total_lines - scanner.buffer_lines;
  while ((index = scanner.current_buffer.find("\n", index)) != std::string::npos && line_index != location.begin.line) {
    // Get the line from the buffer, excluding the trailing newline
    line = scanner.current_buffer.substr(previous_index, index - previous_index);
    // Consume the newline
    index++;
    previous_index = index;
    line_index++;
  }

  // Write out the first part of the line
  std::cerr << line.substr(0, location.begin.column - 1);

  // Write out the error as red
  std::cerr << "\033[31m" << line.substr(location.begin.column - 1, location.end.column - location.begin.column) << "\033[0m";

  // Write out the rest of the line
  std::cerr << line.substr(location.end.column - 1) << std::endl;

  // Write out an arrow to the error
  std::cerr << "      |   ";
  for (int i = 0; i < location.begin.column - 1; i++)
    std::cerr << (line[i] == '\t' ? '\t' : ' ');
  std::cerr << "\033[31m^~~~~~~\033[0m" << std::endl;
}
