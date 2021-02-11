%skeleton "lalr1.cc"
%require "3.7"
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires {
  #include <string>

  #include "nodes/nodes.hpp"
  using namespace nodes;
}

%code {
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;

  ProgramNode* root;

  extern int node_id;
}

%token KEYWORD_CLASS KEYWORD_PUBLIC KEYWORD_STATIC KEYWORD_EXTENDS
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

%type <std::list<VariableDeclarationNode*>> MethodParameters

%type <std::list<Node*>> Statements Expressions
%type <Node*> Statement Expression

%type <ValueNode*> Value

%type <std::string> Operator

%%

Program : ClassDeclarations { root = new ProgramNode(); root->declarations = $1; }
  | END { root = new ProgramNode(); }
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

MethodDeclaration : KEYWORD_PUBLIC TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT MethodParameters KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT Statements KEYWORD_BRACE_RIGHT { $$ = new MethodDeclarationNode($2, $3); $$->parameters = $5; $$->statements = $8; }
  | KEYWORD_PUBLIC TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT Statements KEYWORD_BRACE_RIGHT { $$ = new MethodDeclarationNode($2, $3); $$->statements = $7; }
  | KEYWORD_PUBLIC TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT MethodParameters KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT KEYWORD_BRACE_RIGHT { $$ = new MethodDeclarationNode($2, $3); $$->parameters = $5; }
  | KEYWORD_PUBLIC TYPE IDENTIFIER KEYWORD_PARENTHESES_LEFT KEYWORD_PARENTHESES_RIGHT KEYWORD_BRACE_LEFT KEYWORD_BRACE_RIGHT { $$ = new MethodDeclarationNode($2, $3); }
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
