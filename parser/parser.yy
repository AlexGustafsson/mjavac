%skeleton "lalr1.cc"
%require "3.5"
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

  #include "ast/nodes.hpp"
  using namespace mjavac::ast;

  // Forward declarations
  namespace mjavac {
    class Scanner;
  }
}

%parse-param { Scanner &scanner }
%parse-param { ProgramNode **root }
%parse-param { bool *recovered_failure }

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

%type <MethodDeclarationNode*> MethodDeclaration
%type <MethodDeclarationNode*> MethodScopeDeclaration

%type <std::list<VariableNode*>> MethodParameters

%type <std::list<Node*>> Statements
%type <Node*> Statement Expression

%type <LoopNode*> Loop

%type <ValueNode*> Value

%type <Operator> BinaryOperator UnaryOperator

%type <TypeNode*> Type

%type <std::list<Node*>> ParameterList

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
  : Type IDENTIFIER ';' { $$ = new VariableNode($1, $2); }
  | Type IDENTIFIER '=' Expression ';' { auto _variable = new VariableNode($1, $2); set_location(_variable, @1, @2); $$ = new BinaryOperationNode(_variable, $4, Operator::Assign); set_location($$, @1, @5); }
  | MethodDeclaration { $$ = $1; }
  ;

MethodDeclaration
  : MethodScopeDeclaration Type IDENTIFIER '(' MethodParameters ')' '{' Statements '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; $$->statements = $8; set_location($$, @1, @8); }
  | MethodScopeDeclaration Type IDENTIFIER '(' ')' '{' Statements '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->statements = $7; set_location($$, @1, @8); }
  | MethodScopeDeclaration Type IDENTIFIER '(' MethodParameters ')' '{' '}' { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; set_location($$, @1, @8); }
  | MethodScopeDeclaration Type IDENTIFIER '(' ')' '{' '}' { $$ = $1; $$->type = $2; $$->identifier = $3; set_location($$, @1, @7); }
  | error '}' { (*recovered_failure = true); /* on error, try to skip the entire method declaration */ }
  ;

MethodScopeDeclaration
  : KEYWORD_PUBLIC KEYWORD_STATIC { $$ = new MethodDeclarationNode(true, true); }
  | KEYWORD_PRIVATE KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  | KEYWORD_PUBLIC { $$ = new MethodDeclarationNode(true, false); }
  | KEYWORD_PRIVATE { $$ = new MethodDeclarationNode(false, false); }
  | KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  ;

MethodParameters
  : Type IDENTIFIER { $$.push_back(new VariableNode($1, $2)); }
  | MethodParameters ',' Type IDENTIFIER { $$ = $1; $$.push_back(new VariableNode($3, $4)); }
  ;

Statements
  : Statement { $$.push_back($1); }
  | Statements Statement { $$ = $1; $$.push_back($2); }
  ;

Statement
  : Conditional { $$ = $1; }
  | Loop { $$ = $1; }
  | Expression ';' { $$ = $1; }
  | Expression '=' Expression ';' { new BinaryOperationNode($1, $3, Operator::Assign); set_location($$, @1, @4); }
  | Type IDENTIFIER '=' Expression ';' { $$ = new BinaryOperationNode(new VariableNode($1, $2), $4, Operator::Assign); set_location($$, @1, @4); }
  | Type IDENTIFIER ';' { $$ = new VariableNode($1, $2); set_location($$, @1, @3); }
  | KEYWORD_RETURN Expression ';' { $$ = new ReturnNode($2); set_location($$, @1, @2); }
  | KEYWORD_RETURN ';' { $$ = new ReturnNode(); set_location($$, @1, @2);}
  | error ';' { (*recovered_failure = true); /* on error, try to skip the entire statement */}
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
  | Value { $$ = $1; }
  | UnaryOperator Expression %prec OPERATOR_NOT { $$ = new UnaryOperationNode($2, $1); set_location($$, @1, @2);}
  | KEYWORD_NEW TYPE '[' Expression ']' { $$ = new ArrayInitializationNode(new TypeNode($2), $4); set_location($$, @1, @5); }
  | KEYWORD_NEW IDENTIFIER '(' ')' { $$ = new ClassInitializationNode($2); set_location($$, @1, @4); }
  | Expression '(' ParameterList ')' { auto _call = new MethodCallNode($1); _call->parameters = $3; $$ = _call; set_location($$, @1, @4); }
  | Expression '(' ')' { $$ = new MethodCallNode($1); set_location($$, @1, @3); }
  | Expression '[' Expression ']' { $$ = new BinaryOperationNode($1, $3, Operator::Subscript); set_location($$, @1, @4); }
  | Expression '.' IDENTIFIER { auto _value = new ValueNode(ValueNode::Identifier, $3); set_location($$, @3, @3); $$ = new BinaryOperationNode($1, _value, Operator::Dot); set_location($$, @1, @3); }
  ;

Value
  : INTEGER { $$ = new ValueNode(ValueNode::Integer, $1); set_location($$, @1, @1); }
  | BOOLEAN { $$ = new ValueNode(ValueNode::Boolean, $1); set_location($$, @1, @1); }
  | IDENTIFIER { $$ = new ValueNode(ValueNode::Identifier, $1); set_location($$, @1, @1); }
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

ParameterList
  : Expression { $$.push_back($1); }
  | ParameterList ',' Expression { $$ = $1; $$.push_back($3); }
  ;

Type
  : TYPE { $$ = new TypeNode($1); }
  | IDENTIFIER { $$ = new TypeNode($1); }
  | TYPE '[' ']' { $$ = new TypeNode($1, true); }
  ;

%%

void mjavac::Parser::error(const location_type &location, const std::string &error) {
  scanner.source->print_line_error(std::cerr, location.begin.line, location.begin.column, error);
  scanner.source->print_marked(std::cerr, location.begin.line, location.begin.column, location.end.line, location.end.column);
}
