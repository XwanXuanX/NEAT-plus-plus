#include "graph-network.hpp"
#include <utility>
#include <cassert>

// construct both graphs from list of connections
void GraphNet::construct(const GraphNet::ConnectionList& connections){
        for(auto connection : connections){
                if(connection.enable){
                        assert(add(connection.in, connection.out, connection.weight));
                }
        }
}

// add an edge to both graphs - if edge already exists, return false
bool GraphNet::add(NodeID in_node, NodeID out_node, const long double weight){
        if(exist(in_node, out_node))
                return false;
        graph[in_node].insert(std::make_pair(out_node, weight));
        Tgraph[out_node].insert(in_node);
        return true;
}

// erase an edge from both graphs - if edge does not exist, return false
bool GraphNet::erase(NodeID in_node, NodeID out_node){
        if(!exist(in_node, out_node))
                return false;
        graph[in_node].erase(out_node);
        Tgraph[out_node].erase(in_node);
        return true;
}

// find all ancestors that can reach the target node via at least one path
std::set<uint64_t> GraphNet::ancestors(NodeID node) const{

}

// find all children that is reachable from the target node via at least one path
std::set<uint64_t> GraphNet::children(NodeID node) const{

}

// return the topological ordering of the WEIGHTED graph
std::vector<uint64_t> GraphNet::topsort() const{

}

// check if there exists a cycle in the WEIGHTED graph
bool GraphNet::has_cycle() const{

}

// count the number of connected components in the WEIGHTED graph
uint64_t GraphNet::components() const{

}

// check if an edge exists
bool GraphNet::exist(NodeID in_node, NodeID out_node) const{
        return (graph.count(in_node) && // in node must first exists and out node exists as a child of in node
                graph.at(in_node).count(out_node));
}
        





#if false
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
#endif
