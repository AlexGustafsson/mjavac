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
  std::istream* stream;

public:
  std::string current_buffer;
  std::string file_name;
  int total_lines;
  int buffer_lines;

  Scanner(std::istream *stream, std::string file_name)
      : yyFlexLexer(stream){ this->stream = stream; this->file_name = file_name; this->total_lines = 0; this->buffer_lines = 0; };

  // Solve warning for override of hidden virtual function
  using FlexLexer::yylex;

  virtual int yylex(mjavac::Parser::semantic_type *const lval, mjavac::Parser::location_type *location);

  virtual int LexerInput(char *buffer, int max_length) {
    this->current_buffer = "";
    this->buffer_lines = 0;
    int i = 0;
    for (; i < max_length && this->stream->get(buffer[i]); i++) {
      if (buffer[i] == '\n') {
        this->total_lines++;
        this->buffer_lines++;
      }

      this->current_buffer += buffer[i];
    }
    return i;
  }

  virtual ~Scanner(){};
};
} // namespace mjavac

#endif
