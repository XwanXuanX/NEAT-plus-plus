#pragma once

#include <string>
#include <cstdint>
#include <utility>

using std::int64_t;

// utility function to format a proper exception message
std::string make_errmsg(const std::string& file, const int line, const std::string& msg);

// utility function to randomly select a number in an inclusive range
int64_t rand_select(const std::pair<int64_t, int64_t> range);
