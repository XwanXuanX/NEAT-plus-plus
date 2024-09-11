#pragma once

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <cstdint>
#include <utility>
#include <filesystem>
#include "gene.hpp"
#include "graph-network.hpp"

// using declarations
using std::uint64_t;

// define genotype
class Genotype{
    public: // public member functions
        using DataPkt = std::map<uint64_t, long double>;
        using NodeList = std::list<Node>;
        using ConnectionList = std::list<Connection>;

        // this constructor creates a network with no hidden nodes
        // inputs and outputs forms a fully connected graph, each edge receives a weight of 1;
        explicit Genotype(const int inputs, const int outputs);
        // another way to construct a genotype is by reading from a .model file
        explicit Genotype(const std::filesystem::path& model_file);

        // using the input data, propogate the network and compute for the output
        DataPkt evaluate(const DataPkt& pkt);

        // randomly mutate the genotype
        void mutate();

    public: // public member variables
        // the score (fitness level) of a genotype
        long double fitness;

    private: // private member function
        friend struct GenotypeProbing; // linking printing utils

        // add random connection mutation - return if the connection is successfully added
        bool add_connection();

        // add random node mutation - return if the node is successfully added
        bool add_node();

        // randomly toggle (disable & enable) a connection - always success
        bool toggle_connection();

    private: // private member variables
        // we would prefer using a linked list to store all the node genes and connection genes
        // linked list support O(1) operations (compared to using vector)
        NodeList node_genes;
        ConnectionList connection_genes;

        // graph-based representation of the network
        GraphNet net;

        // each genotype will receive it's own id number, this is used to differentiate each genes
        inline static uint64_t id_counter = 0;
        uint64_t id;
};
