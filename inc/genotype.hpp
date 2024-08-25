#pragma once

#include <cstdint>
using std::uint64_t;

// define three node types (hidden, input(sensor), output)
enum struct NodeType{
        hidden,
        sensor,
        output
};

// define node genes
struct Node{
        uint64_t node_number;
        NodeType node_type;
};

// define connection genes
struct Connection{
        uint64_t in, out;
        long double weight;
        bool enable;
        uint64_t innov;
};

