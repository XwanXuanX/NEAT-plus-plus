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

void Genotype::dumpfile(const std::string& file_name) const{
        // open the output file, if non, create one
        std::ostringstream full_file_name;
        full_file_name << file_name << ".model";
        std::ofstream outfile(full_file_name.str());
        if(!outfile.is_open()){
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"cannot open target .model file"));
        }

        // write node genes
        // write node number
        outfile << std::to_string(node_genes.size()) << std::endl;
        for(auto& node : node_genes)
                outfile << std::to_string(node.node_number) << ' ';
        outfile << std::endl;
        // write node type
        for(auto& node : node_genes)
                outfile << Node::get_nodetype(node.node_type) << ' ';
        outfile << std::endl;

        // write connection genes
        outfile << std::to_string(connection_genes.size()) << std::endl;
        for(auto& connect : connection_genes)
                outfile << Connection::make_connect(connect) << std::endl;
}

void Genotype::dump(const std::string& file_name) const{
        dumpfile(file_name);
}

void Genotype::dump() const{
        dumpfile(std::to_string(id));
}

void Genotype::generate_dot(const std::filesystem::path& model_file) const {
        // open the dot file, if non, create one
        std::ofstream dotfile(model_file);
        if (!dotfile.is_open()) {
                throw std::runtime_error(make_errmsg(__FILE__, __LINE__, "cannot open target .dot file"));
        }

        // dot file opening line and style settings
        dotfile << "digraph G {\n";
        dotfile << "    node [shape=circle, style=filled, fontname=\"Helvetica\", fontsize=12];\n";
        dotfile << "    edge [fontname=\"Helvetica\", fontsize=10];\n";
        
        // generate node genes
        // generate nodes number and type
        for (auto& node : node_genes) {
                std::string color;
                switch (node.node_type) {
                case NodeType::sensor:
                        color = "green2";
                        break;
                case NodeType::hidden:
                        color = "grey";
                        break;
                case NodeType::output:
                        color = "pink";
                        break;
                }
                dotfile << "    " << node.node_number << " [label=\"" << node.node_number 
                        << " (" << Node::get_nodetype(node.node_type) << ")\", fillcolor=" << color << "];\n";
        }
        
        // generate connection genes
        for (auto& connect : connection_genes) {
                dotfile << "    " << connect.in << " -> " << connect.out 
                        << " [label=\"Weight: " << connect.weight 
                        << "\", color=" << (connect.enable ? "blue" : "red") << "];\n";
        }
        
        dotfile << "}\n";
}

void Genotype::generate_image() const {
        // construct the DOT file name
        std::ostringstream dot_file_path;
        dot_file_path << std::to_string(id) << ".dot";

        // generate the dot file first
        generate_dot(dot_file_path.str());
        
        // construct the PNG file name
        std::ostringstream png_file_path;
        png_file_path << std::to_string(id) << ".png";
        
        // create the command to convert DOT to PNG using Graphviz's dot tool
        std::ostringstream command;
        command << "dot -Tpng -o " << png_file_path.str() << " " << dot_file_path.str();
        
        // execute the command
        int result = std::system(command.str().c_str());
        
        //check if the command was successful
        if (result != 0) {
                throw std::runtime_error(make_errmsg(__FILE__, __LINE__, "cannot generate image from DOT file"));
        }
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

#ifdef DEBUG
// print the node genes for display
void Genotype::print_node() const{
        std::cout << node_genes.size() << std::endl;
        for(auto& node : node_genes)
                std::cout << node.node_number << ' ';
        std::cout << std::endl;
        for(auto& node : node_genes)
                std::cout << Node::get_nodetype(node.node_type) << ' ';
        std::cout << std::endl;
}

// print the connection genes for display
void Genotype::print_connection() const{
        std::cout << connection_genes.size() << std::endl;
        for(auto& connection : connection_genes)
                std::cout << Connection::make_connect(connection) << std::endl;
}
#endif
