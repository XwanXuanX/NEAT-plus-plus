#pragma once

#include <list>
#include <cstdint>
#include <string>
#include <filesystem>
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

// define genotype
class Genotype{
    public:
        // this constructor creates a network with no hidden nodes
        // inputs and outputs forms a fully connected graph, each edge receives a weight of 1;
        explicit Genotype(const int inputs, const int outputs);
        // another way to construct a genotype is by reading from a .model file
        explicit Genotype(const std::filesystem::path& model_file);
        
        // write the node genes and connection genes to a .model file
        void dump() const;
        void dump(const std::string& file_name) const;

    private:
        void dumpfile(const std::string& file_name) const;

        // we would prefer using a linked list to store all the node genes and connection genes
        // linked list support O(1) operations (compared to using vector)
        std::list<Node> node_genes;
        std::list<Connection> connection_genes;

        // each genotype will receive it's own id number, this is used to differentiate each genes
        inline static uint64_t id_counter = 0;
        uint64_t id;

        // we would still need a graph representation of above nodes and edges
        // but more on this later...
};
