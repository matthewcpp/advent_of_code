#include "16.hpp"


#include <cstdint>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>


class Solver {
    struct PathState {
        uint64_t path_mask;
        uint64_t position;
        uint64_t time_remaining;
        uint64_t total_flow;
    };

    struct PathResult {
        uint64_t path_mask;
        uint64_t total_flow;
    };

public:
    uint64_t computeMaxFlow(Tunnels const & tunnels, std::string const & starting_node_name, uint64_t simulation_time) {
        std::vector<PathState> path_queue;
        std::vector<PathResult> path_results;
        size_t starting_node_index = getStartingNodeIndex(tunnels, starting_node_name);
        starting_node_mask = 1ull << starting_node_index;
        step_count = 0;

        path_queue.push_back({(1ull << starting_node_index), starting_node_index, simulation_time, 0});

        while (!path_queue.empty()) {
            auto const current_path = path_queue.back();
            path_queue.pop_back();

            auto const * current_node = tunnels.filtered_graph.nodes[current_path.position].get();

            for (uint64_t i = 0; i < tunnels.filtered_graph.nodes.size(); i++) {
                uint64_t next_node_mask = 1ull << i;
                uint64_t move_and_open_time = tunnels.filtered_graph_distances.distances[current_path.position][i] + 1;

                if (current_path.path_mask & next_node_mask)
                    continue;

                if (move_and_open_time >= current_path.time_remaining)
                    continue;

                uint64_t next_path_mask = current_path.path_mask | next_node_mask;
                uint64_t time_remaining = current_path.time_remaining - move_and_open_time;
                uint64_t total_flow = current_path.total_flow + (tunnels.filtered_graph.nodes[i]->value.flow_rate * time_remaining);

                path_queue.push_back({next_path_mask, i, time_remaining, total_flow});
                path_results.push_back({next_path_mask, total_flow});
                step_count += 1;
            }
        }

        return computeMaxFlowForPaths(path_results);
    }

private:
    uint64_t getStartingNodeIndex(Tunnels const & tunnels, std::string const & starting_node_name) const {
        for (size_t i = 0; i < tunnels.filtered_graph.nodes.size(); i++) {

            if (tunnels.filtered_graph.nodes[i]->value.name == starting_node_name) {
                return i;
            }
        }

        return std::numeric_limits<uint64_t>::max();
    }

    uint64_t computeMaxFlowForPaths(std::vector<PathResult> const & path_results) {
        uint64_t max_flow = 0;

        for (size_t i = 0; i < path_results.size(); i++) {
            for (size_t j = 0; j < path_results.size(); j++) {
                auto const & path_i = path_results[i];
                auto const & path_j = path_results[j];

                if ((path_i.path_mask & path_j.path_mask) != starting_node_mask) {
                    continue;
                }

                max_flow = std::max(max_flow, path_results[i].total_flow + path_results[j].total_flow);
            }
        }

        return max_flow;
    }

    uint64_t starting_node_mask = 0;

public:
    uint64_t step_count = 0;
    
};


int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "16.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "16.txt";

    Tunnels tunnels;
    tunnels.parse(input_file_path);

    const uint64_t simulation_time = 26;

    Solver solver;
    std::cout << solver.computeMaxFlow(tunnels, "AA", simulation_time) << '(' << solver.step_count << ')' <<  std::endl;
    return 0;
}