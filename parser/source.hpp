#ifndef SOURCE_H
#define SOURCE_H

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace mjavac {
class Source {
public:
  struct line_location_t {
    size_t start;
    size_t length;
  };

  std::string buffer;
  std::string_view view;
  std::string file_name;
  std::vector<line_location_t *> line_locations;

  Source(std::string file_name);

  const std::string_view get_line(int line) const;
  const std::string_view get_text(int begin_line, int begin_column, int end_line, int end_column) const;
  void print_line_location(std::ostream &stream, int line, int column) const;
  void print_line_error(std::ostream &stream, int line, int column, std::string error) const;
  void print_line_warning(std::ostream &stream, int line, int column, std::string warning) const;
  void print_marked(std::ostream &stream, int begin_line, int begin_column, int end_line, int end_column) const;
};
} // namespace mjavac

#endif
