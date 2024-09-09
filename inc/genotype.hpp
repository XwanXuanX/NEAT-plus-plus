#pragma once

#include <map>
#include <list>
#include <string>
#include <cstdint>
#include <utility>
#include <filesystem>
#include <set>

// using declarations
using std::uint64_t;
using std::int64_t;

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
    public: // public member functions
        using DataPkt = std::map<uint64_t, long double>;
        // map is a set of pairs itself; easier to write, have move functionalities,
        // and two level indexing makes updating an edge weight O(2logN)
        using WeightedGraph = std::map<uint64_t, std::map<uint64_t, long double>>;
        using UnweightedGraph = std::map<uint64_t, std::set<uint64_t>>;
        using NodeList = std::list<Node>;
        using ConnectionList = std::list<Connection>;

        // this constructor creates a network with no hidden nodes
        // inputs and outputs forms a fully connected graph, each edge receives a weight of 1;
        explicit Genotype(const int inputs, const int outputs);
        // another way to construct a genotype is by reading from a .model file
        explicit Genotype(const std::filesystem::path& model_file);

        // using the input data, propogate the network and compute for the output
        DataPkt evaluate(const DataPkt& pkt);

    public: // public member variables
        // the score (fitness level) of a genotype
        long double fitness;

    private:
        friend struct GenotypeProbing; // linking printing utils

        // we would prefer using a linked list to store all the node genes and connection genes
        // linked list support O(1) operations (compared to using vector)
        NodeList node_genes;
        ConnectionList connection_genes;

        // adjacency list strcture of the network
        WeightedGraph graph;

        // transpose of graph
        UnweightedGraph transpose_graph;

        // helper method to construct graph based on connection list
        void construct_graph(const ConnectionList& connections);

        // each genotype will receive it's own id number, this is used to differentiate each genes
        inline static uint64_t id_counter = 0;
        uint64_t id;
};
