#ifndef ERROR_H
#define ERROR_H

#include <string_view>
#include <source_location>

void error(const std::string_view&      message,
           const std::source_location&  location =
                 std::source_location::current());

#endif
