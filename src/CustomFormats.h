#ifndef __CUSTOMFORMATS_H__
#define __CUSTOMFORMATS_H__

#include <fmt/format.h>

#include <QString>

template <>
struct fmt::formatter<QString> {
  // Presentation format: 's' - string.
  char presentation = 's';

  // Parses format specifications of the form ['s'].
  constexpr auto parse(format_parse_context& ctx)
  {

    // Parse the presentation format and store it in the formatter:
    auto it = ctx.begin();
    auto end = ctx.end();
    if (it != end && (*it == 'f' || *it == 'e')) {
        presentation = *it++;
    }

    // Check if reached the end of the range:
    if (it != end && *it != '}') {
      throw format_error("invalid format");
    }

    // Return an iterator past the end of the parsed range:
    return it;
  }

  // Formats the point p using the parsed format specification (presentation)
  // stored in this formatter.
  template <typename FormatContext>
  auto format(const QString& str, FormatContext& ctx)
  {
    // ctx.out() is an output iterator to write to.
    return format_to(
        ctx.out(),
        "{:s}",
        str.toStdString());
  }
};

#endif /*__CUSTOMFORMATS_H__*/
