#include <iomanip>
#include <string_view>

#include "source.hpp"
using namespace mjavac;

Source::Source(std::string file_name) {
  this->file_name = file_name;
}

const std::string_view Source::get_line(int line) const {
  if (line < 1 || (size_t)line > this->line_locations.size())
    return std::string_view();

  line_location_t *line_location = this->line_locations[line - 1];
  return this->view.substr(line_location->start, line_location->length);
}

const std::string_view Source::get_text(int begin_line, int begin_column, int end_line, int end_column) const {
  if (begin_line < 1 || begin_column < 1 || end_column < 1 || end_line < begin_line || (size_t)end_line > this->line_locations.size())
    return std::string_view();

  line_location_t *begin_line_location = this->line_locations[begin_line - 1];
  line_location_t *end_line_location = this->line_locations[end_line - 1];

  size_t begin = begin_line_location->start + (begin_column - 1);
  size_t end = end_line_location->start + (end_column - 1);

  return this->view.substr(begin, begin - end);
}

void Source::print_line_location(std::ostream &stream, int line, int column) const {
  stream << "\033[1m" << this->file_name << ":" << line << ":" << column << "\033[0m";
}

void Source::print_line_error(std::ostream &stream, int line, int column, std::string error) const {
  this->print_line_location(stream, line, column);
  stream << " \033[1m\033[31merror: \033[0m" << error << std::endl;
}

void Source::print_line_warning(std::ostream &stream, int line, int column, std::string warning) const {
  this->print_line_location(stream, line, column);
  stream << " \033[1m\033[33mwarning: \033[0m" << warning << std::endl;
}

void Source::print_marked(std::ostream &stream, int begin_line, int begin_column, int end_line, int end_column) const {
  for (int i = begin_line; i <= end_line; i++) {
    stream << std::setw(5) << i << std::setw(0) << " |   ";
    std::string_view line = this->get_line(i);

    // Offset leading whitespace
    size_t leading_whitespace = 0;
    for (const char &character : line) {
      if (character == ' ' || character == '\t')
        leading_whitespace++;
      else
        break;
    }

    line = line.substr(leading_whitespace);
    if (i == begin_line)
      begin_column -= leading_whitespace;
    else if (i == end_line)
      end_column -= leading_whitespace;

    if (i == begin_line) {
      int line_end = end_line == begin_line ? end_column : line.size();
      // Write out the first part of the line
      stream << line.substr(0, begin_column - 1);
      // Write out the error as red
      stream << "\033[31m" << line.substr(begin_column - 1, line_end - begin_column) << "\033[0m";
      // Write out the rest of the line
      stream << line.substr(line_end - 1);

      // Write out an arrow to the error
      stream << "      |   ";
      for (int i = 0; i < begin_column - 1; i++)
        stream << (line[i] == '\t' ? '\t' : ' ');
      stream << "\033[31m^~~~~~~\033[0m" << std::endl;
    } else {
      stream << line << std::endl;
    }
  }
}
