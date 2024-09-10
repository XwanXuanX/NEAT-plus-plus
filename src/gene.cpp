#include "gene.hpp"
#include <iomanip>
#include <sstream>
#include <limits>

// return the char representation of node type (when printing the node types)
char Node::get_nodetype(const NodeType type) noexcept{
        if(type == NodeType::hidden) return 'H';
        else if(type == NodeType::output) return 'O';
        return 'S';
}

// from the char representation to node type enum
NodeType Node::get_nodetype(const char type) noexcept{
        if(type == 'H') return NodeType::hidden;
        else if(type == 'O') return NodeType::output;
        return NodeType::sensor;
}

// return the string representation of each connection (when printing connections)
std::string Connection::make_connect(const Connection& connect){
        std::ostringstream connection;
        connection << connect.in << ' ' << connect.out << ' ';
        connection << std::setprecision(std::numeric_limits<long double>::max_digits10) << connect.weight << ' ';
        connection << (connect.enable ? 'E' : 'D') << ' ';
        connection << connect.innov;
        return connection.str();
}
