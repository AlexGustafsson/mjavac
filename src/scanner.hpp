#ifndef SCANNER_H
#define SCANNER_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "location.hh"
#include "parser.tab.hpp"

namespace mjavac {
class Scanner : public yyFlexLexer {
private:
  mjavac::Parser::semantic_type *yylval = nullptr;

public:
  Scanner(std::istream *stream)
      : yyFlexLexer(stream){};

  // Solve warning for override of hidden virtual function
  using FlexLexer::yylex;

  virtual int yylex(mjavac::Parser::semantic_type *const lval, mjavac::Parser::location_type *location);

  virtual ~Scanner(){};
};
} // namespace mjavac

#endif
