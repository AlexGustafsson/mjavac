%skeleton "lalr1.cc"
%require "3.7"
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires {
  #include <string>

  #include "node.hpp"
}

%code {
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;

  Node* root;

  extern int node_id;
}

%token KEYWORD_CLASS KEYWORD_PUBLIC KEYWORD_STATIC KEYWORD_MAIN KEYWORD_EXTENDS
%token KEYWORD_IF KEYWORD_WHILE KEYWORD_PARANTHESES_LEFT KEYWORD_PARANTHESES_RIGHT
%token KEYWORD_BRACKET_LEFT KEYWORD_BRACKET_RIGHT KEYWORD_BRACE_LEFT KEYWORD_BRACE_RIGHT
%token KEYWORD_SEMICOLON KEYWORD_COMMA KEYWORD_THIS KEYWORD_NEW KEYWORD_EQUAL

%token <std::string> INTEGER BOOLEAN TYPE IDENTIFIER

%token OPERATOR_NOT OPERATOR_AND OPERATOR_LESS_THAN OPERATOR_LESS_THAN_OR_EQUAL
%token OPERATOR_GREATER_THAN OPERATOR_GREATER_THAN_OR_EQUAL OPERATOR_PLUS
%token OPERATOR_MINUS OPERATOR_MULTIPLICATION

%token PRINTLN

%token END 0 "end of file"

%type <Node*> expression

%%

expression : END { root = nullptr; }

%%
