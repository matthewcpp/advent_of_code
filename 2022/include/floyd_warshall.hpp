#pragma once

#include "graph.hpp"

#include <cstdint>
#include <limits>
#include <vector>

template<typename GraphType>
class FloydWarshall {
public:
    using WeightType = typename GraphType::WeightType;
    using NodeType = typename GraphType::NodeType;

    void computeDistances(GraphType const & graph) {
        std::unordered_map<NodeType const *, size_t> node_map;

        distances.resize(graph.nodes.size());

        for (auto & row : distances) {
            row.clear();
            row.resize(graph.nodes.size(), std::numeric_limits<const WeightType>::max());
        }

        for (size_t i = 0; i < graph.nodes.size(); i++) {
            auto const* graph_node = graph.nodes[i].get();

            distances[i][i] = static_cast<WeightType>(0);
            node_map[graph_node] = i;
        }

        for (auto const & node : graph.nodes) {
            auto current_node_index = node_map[node.get()];

            for (auto const & edge : node->edges) {
                auto edge_node_index = node_map[edge.node];
                distances[current_node_index][edge_node_index] = edge.weight;
            }
        }

        for (size_t k = 0; k < graph.nodes.size(); k++) {
            for (size_t i = 0; i < graph.nodes.size(); i++) {
                for (size_t j = 0; j < graph.nodes.size(); j++) {
                    if (distances[i][k] != std::numeric_limits<const WeightType>::max() && 
                        distances[k][j] != std::numeric_limits<const WeightType>::max() &&
                        distances[i][j] > distances[i][k] + distances[k][j]) {
                        distances[i][j] = distances[i][k] + distances[k][j];
                    }
                }
            }
        }
    }

    std::vector<std::vector<WeightType>> distances;
};