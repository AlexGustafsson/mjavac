#ifndef SCANNER_H
#define SCANNER_H

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

#include "location.hh"
#include "parser.tab.hpp"
#include "source.hpp"

namespace mjavac {
class Scanner : public yyFlexLexer {
private:
  mjavac::Parser::semantic_type *yylval = nullptr;
  std::istream *stream;
  size_t current_line_length;
  size_t current_line_start;

public:
  Source *source;
  Scanner(std::istream *stream, std::string file_name)
      : yyFlexLexer(stream) {
    this->stream = stream;
    this->source = new Source(file_name);
    this->current_line_length = 0;
    this->current_line_start = 0;
  };

  // Solve warning for override of hidden virtual function
  using FlexLexer::yylex;

  virtual int yylex(mjavac::Parser::semantic_type *const lval, mjavac::Parser::location_type *location);

  virtual int LexerInput(char *buffer, int max_length) {
    int i = 0;
    for (; i < max_length && this->stream->get(buffer[i]); i++) {
      this->current_line_length++;
      if (buffer[i] == '\n') {
        this->source->line_locations.push_back(new Source::line_location_t{this->current_line_start, this->current_line_length});
        this->current_line_start += this->current_line_length;
        this->current_line_length = 0;
      }

      this->source->buffer += buffer[i];
    }

    if (this->stream->eof()) {
      if (this->current_line_length > 0)
        this->source->line_locations.push_back(new Source::line_location_t{this->current_line_start, this->current_line_length});
      this->source->view = std::string_view(this->source->buffer);
    }

    return i;
  }

  virtual ~Scanner(){};
};
} // namespace mjavac

#endif
