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

        // return the char representation of node type (when printing the node types)
        static char get_nodetype(const NodeType type) noexcept;
        // from the char representation to node type enum
        static NodeType get_nodetype(const char type) noexcept;
};

// define connection genes
struct Connection{
        uint64_t in, out;
        long double weight;
        bool enable;
        uint64_t innov;

        // return the string representation of each connection (when printing connections)
        static std::string make_connect(const Connection& connect);
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

        // graph representation of above nodes and edges
        void generate_image() const;

    private:
        void dumpfile(const std::string& file_name) const;

        // we would prefer using a linked list to store all the node genes and connection genes
        // linked list support O(1) operations (compared to using vector)
        std::list<Node> node_genes;
        std::list<Connection> connection_genes;

        // each genotype will receive it's own id number, this is used to differentiate each genes
        inline static uint64_t id_counter = 0;
        uint64_t id;

        // helper method to generate the dot file to generate image
        void generate_dot(const std::filesystem::path& model_file) const;

#define DEBUG
#ifdef DEBUG // debug only methods - to enable, define the DEBUG flag
        void print_node() const;
        void print_connection() const;
#endif
};
