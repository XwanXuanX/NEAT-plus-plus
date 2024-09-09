#include "genotype.hpp"
#include "utility.hpp"
#include "prob.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <limits>
#include <iostream>
#include <deque>
#include <algorithm>

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

        // construct graph representation of the initial network
        construct_graph(connection_genes);
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
                NodeType t = Node::get_nodetype(node_types.at(i));
                node_genes.push_back(Node{.node_number = node_ids.at(i), .node_type = t});
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

        // construct graph representation of the initial network
        construct_graph(connection_genes);
}

// using the input data, propogate the network and compute for the output
Genotype::DataPkt Genotype::evaluate(const Genotype::DataPkt& pkt){
        /**
         * FIXME: FILL ME UP PLS!
         * FIXME: return a dummy value for now...
         */

        return {{1,1}}; // hard-coded for the purpose of testing XorGame class
}

// randomly mutate the genotype
void Genotype::mutate(){
        /**
         * FIXME: FILL ME UP PLS!
         * FIXME: used as a testing method for other types of mutations
         */

        add_node();
        add_node();
        add_node();
        add_node();
        add_node();
        add_node();
        add_node();
        add_node();
        add_node();
        add_node();
        add_connection();
        add_connection();
        add_connection();
        add_connection();
        add_connection();
        add_connection();
        add_connection();
        add_connection();
}

// helper method to construct graph based on connection list
void Genotype::construct_graph(const ConnectionList& connections){
        for(auto connection : connections){
                if(connection.enable){
                        graph[connection.in].insert(std::make_pair(connection.out, connection.weight));
                        Tgraph[connection.out].insert(connection.in);
                }
        }
}

// helper method to generate the topological ordering of the graph
auto Genotype::top_sort() const -> std::vector<uint64_t>{
        // this method use Kahn's algorithm for topological ordering

        std::vector<uint64_t> indeg(node_genes.size() + 1, 0); // node id start from 1
        // calculate the in degree of each vertex
        for(auto& connection : connection_genes)
                if(connection.enable)
                        indeg[connection.out]++;
        
        // obtain all the nodes with in degree 0
        std::deque<uint64_t> q;
        for(auto& node : node_genes)
                if(indeg[node.node_number] == 0)
                        q.push_back(node.node_number);
        
        std::vector<uint64_t> res;
        while(!q.empty()){
                uint64_t node = q.front();
                q.pop_front();
                res.push_back(node);
                // decrease the degree of adjacent nodes; if no adjacent nodes (ie. output nodes) skip
                if(!graph.count(node))
                        continue;
                for(auto& adj : graph.at(node)){
                        indeg[adj.first]--;
                        if(indeg[adj.first] == 0)
                                q.push_back(adj.first);
                }
        }

        // check for a cycle
        if(res.size() != node_genes.size())
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"graph contains cycle(s)!"));

        return res;
}

// add random connection mutation - return if the connection is successfully added
bool Genotype::add_connection(){
        /**
         * randomly generate the in node and the out node
         * out node can be any hidden nodes or output nodes
         * in node can be any sensor nodes or hidden nodes
         * another requirement is that the index or the out node must be strictly greater than that of the in node,
         * in the topological ordering
         */

        // if no hidden nodes, then the graph is already fully connected, no connections can be added
        bool has_hidden = std::any_of(node_genes.begin(), node_genes.end(),
                [this](const Node& node){ return node.node_type == NodeType::hidden;});
        if(!has_hidden)
                return false;

        auto generate_in = [this](){
                std::vector<uint64_t> can;
                for(auto node : node_genes)
                        if(node.node_type != NodeType::output)
                                can.push_back(node.node_number);
                // randomly select one candidate
                return can.at(rand_select({0, can.size() - 1}));
        };

        auto generate_out = [this](const uint64_t in_node){
                std::set<uint64_t> reachable;
                std::deque<uint64_t> q;
                q.push_back(in_node);

                while(!q.empty()){
                        uint64_t node = q.front();
                        q.pop_front();
                        if(reachable.count(node))
                                continue;
                        reachable.insert(node);
                        if(!Tgraph.count(node))
                                continue;
                        for(auto adj : Tgraph.at(node))
                                q.push_back(adj);
                }

                std::vector<uint64_t> can;
                for(auto node : node_genes)
                        if(node.node_type != NodeType::sensor && !reachable.count(node.node_number))
                                can.push_back(node.node_number);
                
                return can.at(rand_select({0, can.size() - 1}));
        };

        uint64_t in_node = generate_in();
        uint64_t out_node = generate_out(in_node);

        // check if the connection already exists
        if(graph.at(in_node).count(out_node)) // fast method - check for enabled connections
                return false;
        for(auto& connection : connection_genes) // slow method - check all connections, including the disabled ones
                if(in_node == connection.in && out_node == connection.out)
                        return false;

        // add the new connection
        connection_genes.push_back(Connection{
                .in = in_node, .out = out_node, .weight = 1,
                .enable = true,
                /**
                 * FIXME: implement the actural innovation number later
                 */
                .innov = 1
        });
        graph[in_node].insert(std::make_pair(out_node, 1));
        Tgraph[out_node].insert(in_node);

        // after adding the new connection, validate the new connection does not introduce a cycle
        try{ top_sort(); }
        catch(const std::runtime_error& e){
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"new connection introduce a cycle"));
        }

        return true;
}

// add random node mutation - return if the node is successfully added
bool Genotype::add_node() {
        // check if there are existing connections 
        if(connection_genes.empty())
                return false;

        // generate a random connection to add node
        auto it = connection_genes.begin();
        std::advance(it, rand_select({0, connection_genes.size() - 1}));
        Connection& connection = *it;
        
        // disable the selected connection
        if (connection.enable)
                connection.enable = false;
        else
                return false;

        // create a new hidden node
        Node new_node{.node_number = node_genes.size() + 1, .node_type = NodeType::hidden};
        node_genes.push_back(new_node);

        // create two new connections
        // first connection: from input node of the disabled connection to the new node
        connection_genes.push_back(Connection{
                .in = connection.in, .out = new_node.node_number, .weight = 1.0, .enable = true,
                /**
                * FIXME: each gene should pick up a new innovation number.
                * FIXME: this is a placeholder, should be implemented later!
                */
                .innov = 1
        });

        // second connection: from the new node to the original output node
        connection_genes.push_back(Connection{
                .in = new_node.node_number, .out = connection.out, .weight = connection.weight, .enable = true,
                /**
                * FIXME: each gene should pick up a new innovation number.
                * FIXME: this is a placeholder, should be implemented later!
                */
                .innov = 1
        });

        // update the graph with the new connections
        // remove the disabled connection
        graph[connection.in].erase(connection.out);
        Tgraph[connection.out].erase(connection.in);
        // add the connection from input node of the connection to the new node to the graph
        graph[connection.in].insert(std::make_pair(new_node.node_number, 1.0));
        Tgraph[new_node.node_number].insert(connection.in);
        // add the second new connection to the graph
        graph[new_node.node_number].insert(std::make_pair(connection.out, connection.weight));
        Tgraph[connection.out].insert(new_node.node_number);

        return true;
}
