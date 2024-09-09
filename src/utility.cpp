#include "utility.hpp"
#include <random>

// utility function to format a proper exception message
std::string make_errmsg(const std::string& file, const int line, const std::string& msg){
        std::string errmsg;
        errmsg.append(file), errmsg.append(std::to_string(line)), errmsg.append(msg);
        return errmsg;
}

// utility function to randomly select a number in an inclusive range
int64_t rand_select(const std::pair<int64_t, int64_t> range){
        std::random_device rd;
        std::uniform_int_distribution<int64_t> uid(range.first, range.second);
        return uid(rd);
}
