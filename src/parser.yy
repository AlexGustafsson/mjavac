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

  #include "nodes/nodes.hpp"
  using namespace nodes;

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
%token KEYWORD_IF KEYWORD_WHILE KEYWORD_PARENTHESES_LEFT KEYWORD_PARENTHESES_RIGHT
%token KEYWORD_BRACKET_LEFT KEYWORD_BRACKET_RIGHT KEYWORD_BRACE_LEFT KEYWORD_BRACE_RIGHT
%token KEYWORD_SEMICOLON KEYWORD_COMMA KEYWORD_THIS KEYWORD_NEW KEYWORD_EQUAL KEYWORD_ELSE
%token KEYWORD_DOT

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

ClassDeclaration : KEYWORD_CLASS IDENTIFIER KEYWORD_EXTENDS IDENTIFIER KEYWORD_BRACE_LEFT Declarations KEYWORD_BRACE_RIGHT { $$ = new ClassDeclarationNode($2, $4); $$->setDeclarations($6); }
  | KEYWORD_CLASS IDENTIFIER KEYWORD_BRACE_LEFT Declarations KEYWORD_BRACE_RIGHT { $$ = new ClassDeclarationNode($2); $$->setDeclarations($4); }
  ;

Declarations : Declaration { $$.push_back($1); }
  | Declarations Declaration { $$ = $1; $$.push_back($2); }
  ;

Declaration : VariableDeclaration KEYWORD_SEMICOLON { $$ = $1; }
  | MethodDeclaration { $$ = $1; }
  ;

VariableDeclaration : TYPE KEYWORD_BRACKET_LEFT KEYWORD_BRACKET_RIGHT IDENTIFIER { $$ = new VariableDeclarationNode($1, $4, true); }
  | TYPE IDENTIFIER { $$ = new VariableDeclarationNode($1, $2); }
  ;

MethodDeclaration : MethodScopeDeclaration TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT MethodParameters KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT Statements KEYWORD_BRACE_RIGHT { $$ = $1; $$.type = $2; $$.identifier = $3; $$->parameters = $5; $$->statements = $8; }
  | MethodScopeDeclaration TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT Statements KEYWORD_BRACE_RIGHT { $$ = $1; $$->type = $2; $$->identifier = $3; $$->statements = $7; }
  | MethodScopeDeclaration TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT MethodParameters KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT KEYWORD_BRACE_RIGHT { $$ = $1; $$->type = $2; $$->identifier = $3; $$->parameters = $5; }
  | MethodScopeDeclaration TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT KEYWORD_BRACE_RIGHT { $$ = $1; $$->type = $2; $$->identifier = $3; }
  ;

MethodScopeDeclaration : KEYWORD_PUBLIC KEYWORD_STATIC { $$ = new MethodDeclarationNode(true, true); }
  | KEYWORD_PRIVATE KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  | KEYWORD_PUBLIC { $$ = new MethodDeclarationNode(true, false); }
  | KEYWORD_PRIVATE { $$ = new MethodDeclarationNode(false, false); }
  | KEYWORD_STATIC { $$ = new MethodDeclarationNode(false, true); }
  ;

MethodParameters : VariableDeclaration { $$.push_back($1); }
  | MethodParameters KEYWORD_COMMA VariableDeclaration { $$ = $1; $1.push_back($3); }
  ;

Statements : Statement { $$.push_back($1); }
  | Statements Statement { $$ = $1; $$.push_back($2); }
  ;

Statement : KEYWORD_IF KEYWORD_PARENTHESES_LEFT Expression KEYWORD_PARENTHESES_RIGHT Statement KEYWORD_ELSE Statement { $$ = new ConditionalNode($3, $5, $7); }
  | KEYWORD_WHILE KEYWORD_PARENTHESES_LEFT Expression KEYWORD_PARENTHESES_RIGHT Statement { $$ = new LoopNode($3, $5); }
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
  std::cerr << "Parser error: " << error  << '\n'
            << "  begin at line " << location.begin.line <<  " col " << location.begin.column  << '\n'
            << "  end   at line " << location.end.line <<  " col " << location.end.column << "\n";
}
