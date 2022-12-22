#pragma once

#include "graph.hpp"
#include "floyd_warshall.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

struct Valve {
    Valve(std::string const & n) : name(n) {}
    std::string name;
    int flow_rate = 0;
    std::vector<Valve*> connections;
};

struct Tunnels {
    using GraphType = Graph<Valve, uint32_t>;
    using ValveNode = GraphType::Node;

    GraphType graph, filtered_graph;
    std::unordered_map<std::string, ValveNode*> valves;
    ValveNode* start_node;
    
    void parse(const std::string & input_file_path) {
        std::ifstream input_file(input_file_path);
        std::string current_line;
        std::regex parse_regex ("Valve (.+) has flow rate=(\\d+); tunnels* leads* to valves* ([\\w\\s,]*)");
        std::smatch results;

        while (std::getline(input_file, current_line)) {
            std::regex_match(current_line, results, parse_regex);

            std::string valve_name = results[1];
            int flow_rate = std::stoi(results[2]);
            std::string connection_list = results[3];

            auto valve_node = findOrCreateValveNode(valve_name);
            valve_node->value.flow_rate = flow_rate;
            parseConnections(valve_node, connection_list);

            if (valve_name == "AA") {
                start_node = valve_node;
            }
        }

        buildFilteredGraph();
    }

private:
    void parseConnections(ValveNode* valve_node, std::string const connection_list) {
        std::istringstream connection_str(connection_list);

        std::string connection;
        while (std::getline(connection_str, connection, ',')) {
            auto trim_pos = connection.find_last_of(' ');

            if (trim_pos != std::string::npos) {
                connection = connection.substr(trim_pos + 1);
            }

            auto* connection_node = findOrCreateValveNode(connection);
           valve_node->createEdge(connection_node, 1U);
        }
    }

    ValveNode* findOrCreateValveNode(std::string const & name) {
        ValveNode* valve_node = nullptr;

        auto result = valves.find(name);
        if (result == valves.end()) {
            valve_node = graph.emplace(name);
            valves[name] = valve_node;
        }
        else {
            valve_node = result->second;
        }

        return valve_node;
    }

    void buildFilteredGraph() {
        FloydWarshall<Tunnels::GraphType> floyd_warshall;
        floyd_warshall.computeDistances(graph);

        std::unordered_map<size_t, size_t> source_to_filtered;
        std::unordered_map<size_t, size_t> filtered_to_source;

        for (size_t i = 0; i < graph.nodes.size(); i++) {
            auto const * node = graph.nodes[i].get();

            if (node->value.flow_rate > 0 || node == start_node) {
                source_to_filtered[i] = filtered_graph.nodes.size();
                filtered_to_source[filtered_graph.nodes.size()] = i;
                filtered_graph.createNode(node->value);
            }
        }

        for (size_t i = 0; i < filtered_graph.nodes.size(); i++) {
            auto source_node_index = filtered_to_source[i];

            for (size_t j = 0; j < filtered_graph.nodes.size(); j++) {
                auto target_node_index = filtered_to_source[j];

                if (i == j)
                    continue;

                auto source_distance = floyd_warshall.distances[source_node_index][target_node_index];
                filtered_graph.nodes[i]->createEdge(filtered_graph.nodes[j].get(), source_distance);
            }
        }
    }
};

