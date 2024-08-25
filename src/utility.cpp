#include "utility.hpp"

// utility function to format a proper exception message
std::string make_errmsg(const std::string& file, const int line, const std::string& msg){
        std::string errmsg;
        errmsg.append(file), errmsg.append(std::to_string(line)), errmsg.append(msg);
        return errmsg;
}
