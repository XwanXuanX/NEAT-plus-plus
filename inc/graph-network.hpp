#pragma once

#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <cstdint>
#include <type_traits>
#include "gene.hpp"

// ASSUME ALL GRAPHS ARE DIRECTED!
class GraphNet{
    public:
        // map is a set of pairs itself; easier to write, have move functionalities,
        // and two level indexing makes updating an edge weight O(2logN)
        using WeightedGraph = std::map<uint64_t, std::map<uint64_t, long double>>;
        using UnweightedGraph = std::map<uint64_t, std::set<uint64_t>>;
        using ConnectionList = std::list<Connection>;
        using NodeID = const std::uint64_t;

        // construct both graphs from list of connections
        void construct(const ConnectionList& connections);

        // add an edge to both graphs - if edge already exists, return false
        bool add(NodeID in_node, NodeID out_node, const long double weight);

        // erase an edge from both graphs - if edge does not exist, return false
        bool erase(NodeID in_node, NodeID out_node);

        // find all ancestors that can reach the target node via at least one path
        std::set<uint64_t> ancestors(NodeID node) const;

        // find all children that is reachable from the target node via at least one path
        std::set<uint64_t> children(NodeID node) const;

        // return the topological ordering of the WEIGHTED graph
        std::vector<uint64_t> topsort() const;

        // check if there exists a cycle in the WEIGHTED graph
        bool has_cycle() const;

        // check if an edge exists
        bool exist(NodeID in_node, NodeID out_node) const;
        
    private:
        // helper method to find reachable nodes in either graphs
        template<typename G>
        inline auto find_reachable(NodeID node, const G& g) const -> std::set<uint64_t> {
                std::set<uint64_t> reachable;
                std::deque<uint64_t> q;
                q.push_back(node);

                while(!q.empty()){
                        std::uint64_t node = q.front();
                        q.pop_front();
                        if(reachable.count(node))
                                continue;
                        reachable.insert(node);

                        // add adjcent nodes
                        if(!g.count(node))
                                continue;
                        for(auto adj : g.at(node)){
                                q.push_back([]<typename T>(const T& v){
                                        if constexpr(std::is_convertible_v<T, GraphNet::UnweightedGraph::mapped_type::value_type>)
                                                return v;
                                        else if constexpr(std::is_convertible_v<T, GraphNet::WeightedGraph::mapped_type::value_type>)
                                                return v.first;
                                        else
                                                ;
                                }(adj));
                        }
                }

                return reachable;
        }
        
        // adjacency list strcture of the network
        WeightedGraph graph;
        UnweightedGraph Tgraph;

        // keep track of the number of nodes in the network
        uint64_t node_count = 0;
};
