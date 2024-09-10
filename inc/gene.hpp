#pragma once

#include <string>
#include <cstdint>

// define three node types (hidden, input(sensor), output)
enum struct NodeType{
        hidden,
        sensor,
        output
};

// define node genes
struct Node{
        std::uint64_t node_number;
        NodeType node_type;

        // return the char representation of node type (when printing the node types)
        static char get_nodetype(const NodeType type) noexcept;
        // from the char representation to node type enum
        static NodeType get_nodetype(const char type) noexcept;
};

// define connection genes
struct Connection{
        std::uint64_t in, out;
        long double weight;
        bool enable;
        std::uint64_t innov;

        // return the string representation of each connection (when printing connections)
        static std::string make_connect(const Connection& connect);
};
