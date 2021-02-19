%skeleton "lalr1.cc"
%require "3.7"
%language "c++"
%defines
%define api.namespace {mjavac}
%define api.parser.class {Parser}
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

  void set_location(Node *node, const mjavac::Parser::location_type& begin, const mjavac::Parser::location_type& end) {
    if (node == nullptr)
      return;

    node->location.start_line = begin.begin.line;
    node->location.start_column = begin.begin.column;
    node->location.end_line = end.end.line;
    node->location.end_column = end.end.column;
    node->location.defined = true;
  }
}

%token KEYWORD_CLASS KEYWORD_PUBLIC KEYWORD_PRIVATE KEYWORD_STATIC KEYWORD_EXTENDS
%token KEYWORD_IF KEYWORD_WHILE KEYWORD_RETURN KEYWORD_NEW KEYWORD_ELSE

%token <std::string> INTEGER BOOLEAN TYPE IDENTIFIER

%token OPERATOR_NOT OPERATOR_AND OPERATOR_LESS_THAN OPERATOR_LESS_THAN_OR_EQUAL
%token OPERATOR_GREATER_THAN OPERATOR_GREATER_THAN_OR_EQUAL OPERATOR_PLUS OPERATOR_DIVISION
%token OPERATOR_MINUS OPERATOR_MULTIPLICATION OPERATOR_OR OPERATOR_EQUALS OPERATOR_NOT_EQUALS

%token END 0 "end of file"

%left OPERATOR_OR
%left OPERATOR_AND
%left OPERATOR_LESS_THAN OPERATOR_GREATER_THAN OPERATOR_LESS_THAN_OR_EQUAL OPERATOR_GREATER_THAN_OR_EQUAL OPERATOR_NOT_EQUALS OPERATOR_EQUALS
%left OPERATOR_PLUS OPERATOR_MINUS
%left OPERATOR_MULTIPLICATION OPERATOR_DIVISION
%left OPERATOR_NOT

%type <ProgramNode*> Program

%type <std::list<ClassDeclarationNode*>> ClassDeclarations
%type <ClassDeclarationNode*> ClassDeclaration

%type <std::list<Node*>> Declarations
%type <Node*> Declaration

%type <VariableNode*> VariableDeclaration
%type <MethodDeclarationNode*> MethodDeclaration
%type <MethodDeclarationNode*> MethodScopeDeclaration

%type <std::list<VariableNode*>> MethodParameters

%type <std::list<Node*>> Statements
%type <Node*> Statement Expression Chainable

%type <LoopNode*> Loop

%type <ValueNode*> Value

%type <Operator> BinaryOperator UnaryOperator

%type <MethodCallNode*> MethodCall

%type <std::list<std::string>> ObjectList
%type <std::string> Object Type

%type <std::list<Node*>> ParameterList
%type <Node*> Parameter

%type <ConditionalNode*> Conditional ConditionalIf ConditionalElse

%locations
%start Program

%%

Program
  : ClassDeclarations { *root = new ProgramNode(scanner.source); (*root)->declarations = $1; set_location(*root, @1, @1); }
  | END { *root = new ProgramNode(scanner.source); set_location(*root, @1, @1); }
  ;

ClassDeclarations : ClassDeclaration { $$.push_back($1); }
  | ClassDeclarations ClassDeclaration { $$ = $1; $$.push_back($2); }
  ;

ClassDeclaration
  : KEYWORD_CLASS IDENTIFIER KEYWORD_EXTENDS IDENTIFIER '{' Declarations '}' { $$ = new ClassDeclarationNode($2, $4); $$->setDeclarations($6); set_location($$, @1, @7); }
  | KEYWORD_CLASS IDENTIFIER KEYWORD_EXTENDS IDENTIFIER '{' '}' { $$ = new ClassDeclarationNode($2, $4); set_location($$, @1, @6); }
  | KEYWORD_CLASS IDENTIFIER '{' Declarations '}' { $$ = new ClassDeclarationNode($2); $$->setDeclarations($4); set_location($$, @1, @5); }
  | KEYWORD_CLASS IDENTIFIER '{' '}' { $$ = new ClassDeclarationNode($2); set_location($$, @1, @4); }
  ;

Declarations
  : Declaration { $$.push_back($1); }
  | Declarations Declaration { $$ = $1; $$.push_back($2); }
  ;

Declaration
  : VariableDeclaration ';' { $$ = $1; }
  | MethodDeclaration { $$ = $1; }
  ;

VariableDeclaration
  : Type '[' ']' IDENTIFIER { $$ = new VariableNode($1, $4, true, true); set_location($$, @1, @4); }
  | Type IDENTIFIER { $$ = new VariableNode($1, $2, true); set_location($$, @1, @2); }
  ;

MethodDeclaration
  : MethodScopeDeclaration Type IDENTIFIER '(' MethodParameters ')' '{' Statements '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; $$->statements = $8; set_location($$, @1, @8); }
  | MethodScopeDeclaration Type IDENTIFIER '(' ')' '{' Statements '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->statements = $7; set_location($$, @1, @8); }
  | MethodScopeDeclaration Type IDENTIFIER '(' MethodParameters ')' '{' '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; set_location($$, @1, @8); }
  | MethodScopeDeclaration Type IDENTIFIER '(' ')' '{' '}' { $$ = $1; $$->type = $2; $$->identifier = $3; set_location($$, @1, @7); }
  | error '}' /* on error, try to skip the entire method declaration */
  ;

MethodScopeDeclaration
  : KEYWORD_PUBLIC KEYWORD_STATIC { $$ = new MethodDeclarationNode(true, true); }
  | KEYWORD_PRIVATE KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  | KEYWORD_PUBLIC { $$ = new MethodDeclarationNode(true, false); }
  | KEYWORD_PRIVATE { $$ = new MethodDeclarationNode(false, false); }
  | KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  ;

MethodParameters
  : VariableDeclaration { $$.push_back($1); }
  | MethodParameters ',' VariableDeclaration { $$ = $1; $1.push_back($3); }
  ;

Statements
  : Statement { $$.push_back($1); }
  | Statements Statement { $$ = $1; $$.push_back($2); }
  ;

Statement
  : Conditional { $$ = $1; }
  | Loop { $$ = $1; }
  | Expression ';' { $$ = $1; }
  | VariableDeclaration '=' Expression ';' { $$ = $1; $1->assigned_value = $3; set_location($$, @1, @4); }
  | VariableDeclaration ';' { $$ = $1; set_location($$, @1, @2); }
  | Value '=' Expression ';' { $$ = new BinaryOperationNode($1, $3, Operator::Assign); set_location($$, @1, @4); }
  | KEYWORD_RETURN Expression ';' { $$ = new ReturnNode($2); set_location($$, @1, @2); }
  | KEYWORD_RETURN ';' { $$ = new ReturnNode(); set_location($$, @1, @2);}
  | error ';' /* on error, try to skip the entire statement */
  ;

Conditional
  : ConditionalIf
  | ConditionalIf ConditionalElse { $$ = $1; $$->next = $2; }
  ;

ConditionalIf
  : KEYWORD_IF '(' Expression ')' Statement { $$ = new ConditionalNode($3, $5); set_location($$, @1, @5); }
  | KEYWORD_IF '(' Expression ')' '{' Statements '}' { $$ = new ConditionalNode($3, $6); set_location($$, @1, @7); }
  ;

ConditionalElse
  : KEYWORD_ELSE Statement { $$ = new ConditionalNode($2); set_location($$, @1, @2); }
  | KEYWORD_ELSE '{' Statements '}' { $$ = new ConditionalNode($3); set_location($$, @1, @4); }
  ;

Loop
  : KEYWORD_WHILE '(' Expression ')' '{' Statements '}' { $$ = new LoopNode($3); $$->statements = $6; set_location($$, @1, @7); }
  | KEYWORD_WHILE '(' Expression ')' Statement { $$ = new LoopNode($3); $$->statements.push_back($5); set_location($$, @1, @5); }
  ;

Expression
  : Expression BinaryOperator Expression { $$ = new BinaryOperationNode($1, $3, $2); set_location($$, @1, @3); }
  | '(' Expression ')' { $$ = $2; set_location($$, @1, @3); }
  | Chainable { $$ = $1; set_location($$, @1, @1); }
  | Chainable '.' MethodCall { $$ = new BinaryOperationNode($1, $3, Operator::Dot); set_location($$, @1, @3); }
  | Value { $$ = $1; }
  | Value '[' Expression ']' { $$ = $1; $1->is_array = true; $1->array_index = $3; set_location($$, @1, @4); }
  | Chainable '[' Expression ']' { $$ = new BinaryOperationNode($1, $3, Operator::Subscript); set_location($$, @1, @4); }
  | UnaryOperator Expression %prec OPERATOR_NOT { $$ = new UnaryOperationNode($2, $1); set_location($$, @1, @2);}
  ;

Chainable
  : MethodCall { $$ = $1; }
  | KEYWORD_NEW MethodCall { $$ = $2; $2->is_new = true; }
  ;

Value
  : INTEGER { $$ = new ValueNode(ValueNode::Integer, $1); set_location($$, @1, @1); }
  | BOOLEAN { $$ = new ValueNode(ValueNode::Boolean, $1); set_location($$, @1, @1); }
  | IDENTIFIER { $$ = new ValueNode(ValueNode::Identifier, $1); set_location($$, @1, @1); }
  | ObjectList { $$ = new ValueNode(ValueNode::Object, $1); }
  ;

BinaryOperator
  : OPERATOR_AND { $$ = Operator::And; }
  | OPERATOR_OR { $$ = Operator::Or; }
  | OPERATOR_EQUALS { $$ = Operator::Equal; }
  | OPERATOR_NOT_EQUALS { $$ = Operator::NotEqual; }
  | OPERATOR_LESS_THAN { $$ = Operator::LessThan; }
  | OPERATOR_LESS_THAN_OR_EQUAL { $$ = Operator::LessThanOrEqual; }
  | OPERATOR_GREATER_THAN { $$ = Operator::GreaterThan; }
  | OPERATOR_GREATER_THAN_OR_EQUAL { $$ = Operator::GreaterThanOrEqual; }
  | OPERATOR_PLUS { $$ = Operator::Plus; }
  | OPERATOR_MINUS { $$ = Operator::Minus; }
  | OPERATOR_MULTIPLICATION { $$ = Operator::Multiplication; }
  | OPERATOR_DIVISION { $$ = Operator::Division; }
  ;

UnaryOperator
  : OPERATOR_MINUS { $$ = Operator::Negative; }
  | OPERATOR_NOT { $$ = Operator::Negate; }
  ;

MethodCall
  : Value '(' ParameterList ')' { $$ = new MethodCallNode(); $$->value = $1; $$->parameters = $3; set_location($$, @1, @4); }
  | Value '(' ')' { $$ = new MethodCallNode(); $$->value = $1; set_location($$, @1, @3); }
  ;

ObjectList
  : IDENTIFIER { $$.push_back($1); }
  | ObjectList '.' IDENTIFIER { $$ = $1; $$.push_back($3); }
  ;

ParameterList
  : Expression { $$.push_back($1); }
  | ParameterList ',' Expression { $$ = $1; $$.push_back($3); }
  ;

Type
  : TYPE
  | IDENTIFIER
  ;

%%

void mjavac::Parser::error(const location_type &location, const std::string &error) {
  scanner.source->print_line_error(std::cerr, location.begin.line, location.begin.column, error);
  scanner.source->print_marked(std::cerr, location.begin.line, location.begin.column, location.end.line, location.end.column);
}
