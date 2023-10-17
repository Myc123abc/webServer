#include "error.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

void error(const std::string_view&      message,
           const std::source_location&  location) {
    std::cerr << "[ERROR]"
        << location.file_name()     << ": "
        << location.line()          << ": "
        << location.function_name() << ": "
        << message                  << ": "
        << strerror(errno)          << '\n';
    exit(EXIT_FAILURE);
}
