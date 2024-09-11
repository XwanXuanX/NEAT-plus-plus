#include "graph-network.hpp"
#include "utility.hpp"
#include <utility>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <deque>

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
        // add to both graphs
        graph[in_node].insert(std::make_pair(out_node, weight));
        Tgraph[out_node].insert(in_node);

        // keep updating the highest node number when adding edges
        // new node must be added through adding edges
        node_count = std::max(node_count, std::max(in_node, out_node));

        return true;
}

// erase an edge from both graphs - if edge does not exist, return false
bool GraphNet::erase(NodeID in_node, NodeID out_node){
        if(!exist(in_node, out_node))
                return false;
        // erase from both graphs
        graph[in_node].erase(out_node);
        Tgraph[out_node].erase(in_node);

        return true;
}

// find all ancestors that can reach the target node via at least one path
std::set<uint64_t> GraphNet::ancestors(NodeID node) const{
        return find_reachable(node, Tgraph);
}

// find all children that is reachable from the target node via at least one path
std::set<uint64_t> GraphNet::children(NodeID node) const{
        return find_reachable(node, graph);
}

// return the topological ordering of the WEIGHTED graph
std::vector<uint64_t> GraphNet::topsort() const{
        // this method use Kahn's algorithm for topological ordering
        std::vector<uint64_t> indeg(node_count + 1, 0); // node id start from 1
        // calculate the in degree of each vertex
        for(auto& [in, outs] : graph)
                for(auto& out : outs)
                        indeg.at(out.first)++;
        
        // obtain all the nodes with in degree 0
        std::deque<uint64_t> q;
        for(uint64_t node = 1; node <= node_count; ++node)
                if(indeg[node] == 0)
                        q.push_back(node);

        // performing BFS        
        std::vector<uint64_t> res;
        while(!q.empty()){
                uint64_t node = q.front();
                q.pop_front();
                res.push_back(node);
                // decrease the degree of adjacent nodes; if no adjacent nodes (ie. output nodes) skip
                if(!graph.count(node))
                        continue;
                for(auto& adj : graph.at(node)){
                        indeg.at(adj.first)--;
                        if(indeg.at(adj.first) == 0)
                                q.push_back(adj.first);
                }
        }

        // check for a cycle
        if(res.size() != node_count)
                throw std::runtime_error(make_errmsg(__FILE__,__LINE__,"graph contains cycle(s)!"));
        
        return res;
}

// check if there exists a cycle in the WEIGHTED graph
bool GraphNet::has_cycle() const{
        // this method use DFS to detect a cycle in a directed graph
        // a cycle exists iff exists at least one back edge, so, you know...

        std::set<uint64_t> vis;
        // the recursive call stack tracks all the visited nodes in the current dfs run
        // if have an edge pointing back to one of the visited nodes, it's a back edge
        std::set<uint64_t> stk;

        std::function<bool(const uint64_t node)> dfs;
        dfs = [&, this](const uint64_t node) -> bool {
                if(!vis.count(node)){
                        vis.insert(node);
                        // if the node has no outgoing edges, simply end this search
                        if(!graph.count(node))
                                return false;
                        stk.insert(node);

                        for(auto& adj : graph.at(node)){
                                if(!vis.count(adj.first) && dfs(adj.first))
                                        return true;
                                else if(stk.count(adj.first))
                                        return true;
                        }
                }

                // remove the node from the call stack
                stk.erase(node);
                return false;
        };

        for(uint64_t node = 1; node <= node_count; ++node)
                if(!vis.count(node) && dfs(node))
                        return true;
        
        return false;
}

// check if an edge exists
bool GraphNet::exist(NodeID in_node, NodeID out_node) const{
        return (graph.count(in_node) && // in node must first exists and out node exists as a child of in node
                graph.at(in_node).count(out_node));
}
