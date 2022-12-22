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

public:
    uint64_t computeMaxFlow(Tunnels const & tunnels, std::string const & starting_node_name, uint64_t simulation_time) {
        std::vector<PathState> path_queue;
        std::unordered_map<Tunnels::ValveNode const *, size_t> node_lookup;
        size_t starting_node_index = tunnels.filtered_graph.nodes.size();
        uint64_t max_flow = 0;
        step_count = 0;
        
        for (size_t i = 0; i < tunnels.filtered_graph.nodes.size(); i++) {
            auto const * current_node = tunnels.filtered_graph.nodes[i].get();
            node_lookup[current_node] = i;

            if (current_node->value.name == starting_node_name) {
                starting_node_index = i;
            }
        }

        path_queue.push_back({(1ull << starting_node_index), starting_node_index, simulation_time, 0});

        while (!path_queue.empty()) {
            auto const current_path = path_queue.back();
            path_queue.pop_back();

            auto const * current_node = tunnels.filtered_graph.nodes[current_path.position].get();

            for (auto const & edge : current_node->edges) {
                size_t next_node_index = node_lookup[edge.node];
                uint64_t next_node_mask = 1ull << next_node_index;
                uint64_t move_and_open_time = edge.weight + 1;

                if (current_path.path_mask & next_node_mask)
                    continue;

                if (move_and_open_time >= current_path.time_remaining)
                    continue;

                uint64_t time_remaining = current_path.time_remaining - move_and_open_time;
                uint64_t total_flow = current_path.total_flow + (edge.node->value.flow_rate * time_remaining);

                max_flow = std::max(max_flow, total_flow);

                path_queue.push_back({current_path.path_mask | next_node_mask, next_node_index, time_remaining, total_flow});
                step_count += 1;
            }
        }

        return max_flow;
    }

    uint64_t step_count = 0;
};


int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "16.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "16.txt";

    Tunnels tunnels;
    tunnels.parse(input_file_path);

    const uint64_t simulation_time = 30;

    Solver solver;
    std::cout << solver.computeMaxFlow(tunnels, "AA", simulation_time) << '(' << solver.step_count << ')' <<  std::endl;
    return 0;
}