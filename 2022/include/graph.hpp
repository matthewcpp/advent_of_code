#pragma once

#include <cstdint>
#include <memory>
#include <vector>

template <typename Value, typename Weight>
struct Graph {
    struct Node;
    struct Edge {
        Edge (Node* n, Weight w) : node(n), weight(w) {}

        Node* node;
        Weight weight;
    };

    struct Node {
        Node (Value const & val) : value(val) {}

        Value value;
        std::vector<Edge> edges;

        Edge* createEdge(Node* target, Weight w) {
            edges.emplace_back(target, w);
            return &edges.back();
        }
    };

    using ValueType = typename Value;
    using WeightType = typename Weight;
    using NodeType = typename Node;
    using EdgeType = typename Edge;

    Node* createNode(Value const & val) {
        return nodes.emplace_back(std::make_unique<Node>(val)).get();
    }

    template <class... Args>
    Node* emplace(const Args&... args) {
        Value v(args...);
        return createNode(v);
    }

    std::vector<std::unique_ptr<Node>> nodes;
};
