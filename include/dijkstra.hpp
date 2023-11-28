#pragma once

#include "graph.hpp"

#include <map>
#include <unordered_map>
#include <unordered_set>

template <typename GraphType>
struct Dijkstra {
    using GraphNodeType = typename GraphType::Node;
    using GraphWeightType = typename  GraphType::WeightType;
    using DistanceMap = std::unordered_map<GraphNodeType const*, GraphWeightType>;
    using PreviousNodeMap = std::unordered_map<GraphNodeType const*, GraphNodeType const*>;
    using PriorityQueue = std::map<GraphWeightType, std::unordered_set<GraphNodeType const *>>;

    DistanceMap distances;
    PreviousNodeMap previous;
    PriorityQueue queue;

    void computeDistances(GraphType const & graph, GraphNodeType const * start) {
        init(graph, start);

        while (!queue.empty()) {
            auto* active_node = getNextNode();
            auto active_node_dist = distances[active_node];

            for (auto const & edge : active_node->edges) {
                if (!isNodeInQueue(edge.node))
                    continue;
                
                auto path_dist = active_node_dist + edge->weight;
                auto current_dist = distances[edge.node];

                if (path_dist < current_dist) {
                    previous[edge.node] = active_node;

                    auto& current_dist_set = queue[current_dist];
                    current_dist_set.erase(edge.node);
                    queue[path_dist].insert(edge.node);
                    distances[edge.node] = path_dist;

                    if (current_dist_set.empty())
                        queue.erase(current_dist);
                }
            }
        }
    }

private:
    void init(GraphType const & graph, GraphNodeType const * start) {
        distances.clear();
        previous.clear();
        queue.clear();

        for (const auto & item : graph.nodes) {
            GraphNodeType const * node = item.second.get();
            auto initial_val = node == start ? static_cast<GraphWeightType>(0) : std::numeric_limits<GraphWeightType>::max();

            distances[node] = initial_val;
            queue[initial_val].insert(node);
        }

        previous[start] = nullptr;
    }

    GraphNodeType const * getNextNode() {
        if (queue.empty()) {
            return nullptr;
        }

        auto min_value_itr = queue.begin();
        auto& node_set = min_value_itr->second;

        auto node_set_first = node_set.begin();
        auto const * node = *node_set_first;
        node_set.erase(node_set_first);

        if (node_set.empty())
            queue.erase(min_value_itr);

        return node;
    }

    bool isNodeInQueue(GraphNodeType const * node) const {
        auto dist_result = distances.find(node);

        if (dist_result == distances.end())
            return true;

        auto queue_result = queue.find(dist_result->second);
        if (queue_result == queue.end())
            return false;

        return queue_result->second.count(node) > 0;
    }
};
