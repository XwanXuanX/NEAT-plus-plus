#pragma once

#include <string>

// utility function to format a proper exception message
std::string make_errmsg(const std::string& file, const int line, const std::string& msg);
