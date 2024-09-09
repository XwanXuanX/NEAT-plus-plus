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
                node_genes.push_back(Node{.node_number = i, .node_type = NodeType::sensor}), \
                sensor_nodes.insert(i);
        for(uint64_t i = inputs + 1; i <= inputs + outputs; ++i)
                node_genes.push_back(Node{.node_number = i, .node_type = NodeType::output}), \
                output_nodes.insert(i);
        
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
                if(t == NodeType::output)
                        output_nodes.insert(node_ids.at(i));
                else if(t == NodeType::sensor)
                        sensor_nodes.insert(node_ids.at(i));
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

        if(add_connection())
                std::cout << "Connection added successfully\n";
        else
                std::cout << "Connection added failed\n";
}

// helper method to construct graph based on connection list
void Genotype::construct_graph(const ConnectionList& connections){
        for(auto connection : connections)
                if(connection.enable)
                        graph[connection.in].insert(std::make_pair(connection.out, connection.weight));
}

// helper method to generate the topological ordering of the graph
auto Genotype::top_sort() const -> std::vector<uint64_t>{
        // this method use Kahn's algorithm for topological ordering

        std::vector<uint64_t> indeg(node_genes.size() + 1, 0); // node id start from 1
        // calculate the in degree of each vertex
        for(auto& connection : connection_genes)
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

        // find the topological ordering
        std::vector<uint64_t> top_order = top_sort();

        /**
         * by the definition of top sort, top_order's structure will be like this:
         * S S S ... H H H ... O O O ...
         * therefore we can partition the top_order into 3 segments
         */
        auto get_first_hidden = [this](const std::vector<uint64_t>& top_order){
                std::size_t first_hidden{};
                for(std::size_t i = 0; i < top_order.size(); ++i){
                        first_hidden = std::max(first_hidden, i);
                        if(!sensor_nodes.count(top_order.at(i)))
                                break;
                }
                return first_hidden;
        };
        auto get_last_hidden = [this](const std::vector<uint64_t>& top_order){
                std::size_t last_hidden{top_order.size()};
                for(std::size_t i = top_order.size() - 1; i >= 0; --i){
                        last_hidden = std::min(last_hidden, i);
                        if(!output_nodes.count(top_order.at(i)))
                                break;
                }
                return last_hidden;
        };

        std::size_t first_hidden = get_first_hidden(top_order);
        std::size_t last_hidden = get_last_hidden(top_order);
        bool has_hidden = first_hidden <= last_hidden;

        // if no hidden nodes, then the graph is already fully connected, no connections can be added
        if(!has_hidden)
                return false;
        
        // generate the in node of the new connection
        std::size_t in_node_idx = rand_select({0, last_hidden});
        std::size_t out_node_idx = rand_select({std::max(in_node_idx + 1, first_hidden), top_order.size() - 1});
        uint64_t in_node = top_order.at(in_node_idx), out_node = top_order.at(out_node_idx);

        // check if the connection already exists
        if(graph.at(in_node).count(out_node)) // fast method - check for enabled connections
                return false;
        for(auto& connection : connection_genes) // slow method - check all connections, including the disabled ones
                if(in_node == connection.in && out_node == connection.out)
                        return false;
        
        // add the new connection
        connection_genes.push_back(Connection{
                .in = in_node, .out = out_node, .enable = true,
                .weight = 1,
                /**
                 * FIXME: implement the actural innovation number later
                 */
                .innov = 1
        });
        graph[in_node].insert(std::make_pair(out_node, 1));

        // after adding the new connection, validate the new connection does not introduce a cycle
        try{ top_sort(); }
        catch(const std::runtime_error& e){
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"new connection introduce a cycle"));
        }

        return true;
}
