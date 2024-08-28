#include "genotype.hpp"
#include "utility.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <limits>
#include <iostream>

char Node::get_nodetype(const NodeType type) noexcept{
        if(type == NodeType::hidden) return 'H';
        else if(type == NodeType::output) return 'O';
        return 'S';
}

NodeType Node::get_nodetype(const char type) noexcept{
        if(type == 'H') return NodeType::hidden;
        else if(type == 'O') return NodeType::output;
        return NodeType::sensor;
}

std::string Connection::make_connect(const Connection& connect){
        std::ostringstream connection;
        connection << connect.in << ' ' << connect.out << ' ';
        connection << std::setprecision(std::numeric_limits<long double>::max_digits10) << connect.weight << ' ';
        connection << (connect.enable ? 'E' : 'D') << ' ';
        connection << connect.innov;
        return connection.str();
}

// this constructor creates a network with no hidden nodes
// inputs and outputs forms a fully connected graph, each edge receives a weight of 1;
Genotype::Genotype(const int inputs, const int outputs){
        // get a new id number
        id = ++id_counter;

        // create all the nodes
        using std::uint64_t;
        for(uint64_t i = 1; i <= inputs; ++i)
                node_genes.push_back(Node{.node_number = i, .node_type = NodeType::sensor});
        for(uint64_t i = inputs + 1; i <= inputs + outputs; ++i)
                node_genes.push_back(Node{.node_number = i, .node_type = NodeType::output});
        
        // create all the edges
        for(uint64_t i = 1; i <= inputs; ++i){
                for(uint64_t o = inputs + 1; o <= inputs + outputs; ++o){
                        connection_genes.push_back(Connection{
                                .in = i, .out = o, .weight = 1,
                                .enable = true,
                                /**
                                 * FIXME: each gene should pick up a new innovation number.
                                 * FIXME: this is a placeholder, should be implemented later!
                                 */
                                .innov = 1
                        });
                }
        }

        /**
         * FIXME: After adding the graph representation of the network,
         * FIXME: also construct the graph HERE!
         */
}

// another way to construct a genotype is by reading from a .model file
Genotype::Genotype(const std::filesystem::path& model_file){
        using namespace std::filesystem;
        // check if the given path is valid
        if(!exists(model_file) || is_empty(model_file)){
                throw std::invalid_argument(make_errmsg(__FILE__,__LINE__,"cannot open source .model file"));
        }

        // get the absolute path
        path abs_path = absolute(model_file);
        std::ifstream infile(abs_path.c_str());
        if(!infile.is_open()){
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"cannot open source .model file"));
        }

        // get a new id number
        id = ++id_counter;

        char type;
        uint64_t size, node_id;
        infile >> size; // read the number of nodes
        std::vector<uint64_t> node_ids;
        std::vector<char> node_types;
        for(int i = 0; i < size; ++i)
                infile >> node_id, node_ids.push_back(node_id);
        for(int i = 0; i < size; ++i)
                infile >> type, node_types.push_back(type);
        // now using node id and node type create the node gene list
        for(int i = 0; i < size; ++i){
                node_genes.push_back(Node{
                        .node_number = node_ids.at(i),
                        .node_type = Node::get_nodetype(node_types.at(i))
                });
        }

        infile >> size; // read the number of connections
        uint64_t in, out, innov;
        long double weight;
        char enable;

        for(int i = 0; i < size; ++i){
                infile >> in >> out >> weight >> enable >> innov;
                connection_genes.push_back(Connection{
                        .in = in,
                        .out = out,
                        .weight = weight,
                        .enable = enable == 'E' ? true : false,
                        .innov = innov
                });
        }

        /**
         * FIXME: After adding the graph representation of the network,
         * FIXME: also construct the graph HERE!
         */
}
