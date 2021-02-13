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
%token KEYWORD_IF KEYWORD_WHILE KEYWORD_THIS KEYWORD_NEW KEYWORD_ELSE

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

%type <VariableDeclarationNode*> VariableDeclaration
%type <MethodDeclarationNode*> MethodDeclaration
%type <MethodDeclarationNode*> MethodScopeDeclaration

%type <std::list<VariableDeclarationNode*>> MethodParameters

%type <std::list<Node*>> Statements Expressions
%type <Node*> Statement Expression

%type <ValueNode*> Value

%type <std::string> Operator

%locations
%start Program

%%

Program : ClassDeclarations { *root = new ProgramNode(); (*root)->declarations = $1; }
  | END { *root = new ProgramNode(); }
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

VariableDeclaration : TYPE '[' ']' IDENTIFIER { $$ = new VariableDeclarationNode($1, $4, true); }
  | TYPE IDENTIFIER { $$ = new VariableDeclarationNode($1, $2); }
  ;

MethodDeclaration : MethodScopeDeclaration TYPE IDENTIFIER '(' MethodParameters ')' '{' Statements '}' { $$ = $1; $$.type = $2; $$.identifier = $3; $$->parameters = $5; $$->statements = $8; }
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
  | KEYWORD_WHILE '(' Expression ')' Statement { $$ = new LoopNode($3, $5); }
  ;

Expressions : Expression { $$.push_back($1); }
  | Expressions Expression { $$ = $1; $$.push_back($2); }
  ;

Expression : Expression Operator Expression { $$ = new BinaryOperationNode($1, $3, $2); }
  | Value { $$ = $1; }
  ;

Value : INTEGER { $$ = new ValueNode(ValueNode.Integer, $1); }
  | BOOLEAN { $$ = new ValueNode(ValueNode.Boolean, $1); }
  | IDENTIFIER { $$ = new ValueNode(ValueNode.Identifier, $1); }
  ;

Operator : OPERATOR_AND { $$ = Operator.And; }
  | OPERATOR_LESS_THAN { $$ = Operator.LessThan; }
  | OPERATOR_LESS_THAN_OR_EQUAL { $$ = Operator.LessThanOrEqual; }
  | OPERATOR_GREATER_THAN { $$ = Operator.GreaterThan; }
  | OPERATOR_GREATER_THAN_OR_EQUAL { $$ = Operator.GreaterThanOrEqual; }
  | OPERATOR_PLUS { $$ = Operator.Plus; }
  | OPERATOR_MINUS { $$ = Operator.Minus; }
  | OPERATOR_MULTIPLICATION { $$ = Operator.Multiplication; }
  ;

%%

void mjavac::Parser::error(const location_type &location, const std::string &error) {
  std::cerr << "\e[1m" << scanner.file_name << ":" << location.begin.line << ":" << location.begin.column << "\e[0m error: " << error << std::endl;
  std::cerr << std::setw(5) << location.begin.line << std::setw(0) << " |   ";

  // Extract the relevant line from the scanner's buffer
  size_t previous_index = 0;
  size_t index = 0;
  std::string_view line;
  int line_index = scanner.total_lines - scanner.buffer_lines;
  while ((index = scanner.current_buffer.find("\n", index)) != std::string::npos && line_index != location.begin.line) {
    line = scanner.current_buffer.substr(previous_index, index);
    previous_index = index;
    line_index++;
  }

  // Write out the first part of the line
  std::cerr << line.substr(0, location.begin.column - 1);

  // Write out the error as red
  std::cerr << "\e[31m" << line.substr(location.begin.column - 1, location.end.column - location.begin.column) << "\e[0m";

  // Write out the rest of the line
  std::cerr << line.substr(location.end.column - 1) << std::endl;

  // Write out an arrow to the error
  std::cerr << "      |   ";
  for (int i = 0; i < location.begin.column - 1; i++)
    std::cerr << " ";
  std::cerr << "\e[31m^~~~~~~\e[0m" << std::endl;
}
