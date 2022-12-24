#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "12.hpp"

class MultiSolver {
public:
    size_t solve(std::string const & input_file_path, size_t thread_count) {
        map.load(input_file_path);
        get_starting_positions();

        workers.resize(thread_count);

        for (size_t i = 0; i < workers.size(); i++) {
            workers[i] = std::thread(&MultiSolver::threadSolveFunc, this);
        }

        for (auto & worker : workers) {
            worker.join();
        }

        std::sort(results.begin(), results.end());
        return results[0];
    }

private:
    void threadSolveFunc() {
        for (;;) {
            Point2 starting_pos;

            {
                std::lock_guard<std::mutex> lock_starting_positions(starting_positions_mutex);
                if (starting_positions.empty())
                    return;

                starting_pos = starting_positions.back();
                starting_positions.pop_back();
            }
            
            Solver solver(map);
            if (!solver.solve(starting_pos, map.exit))
                continue;

            auto path = solver.getPath();
            auto path_size = path.size();
            {
                std::lock_guard<std::mutex> lock_results(results_mutex);
                results.push_back(path_size > 0 ? path_size - 1 : 0);
            }
        }
    }

    void get_starting_positions() {
        size_t grid_height = map.grid.size();
        size_t grid_width = map.grid[0].size();

        for (size_t y = 0; y < grid_height; y++) {
            auto const & row = map.grid[y];

            for (size_t x = 0; x < grid_width; x ++) {
                if (map.grid[y][x] == 'a') {
                    starting_positions.emplace_back(static_cast<int>(x), static_cast<int>(y));
                }
            }
        }
    }

    Map map;
    std::vector<Point2> starting_positions;
    std::mutex starting_positions_mutex;

    std::vector<size_t> results;
    std::mutex results_mutex;
    std::vector<std::thread> workers;
};


int main(int argc, char** argv) {
    // auto const input_file_path = std::string(INPUT_DIRECTORY) + "12.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "12.txt";

    MultiSolver solver;
    size_t min_steps = solver.solve(input_file_path, 5);

    std::cout << min_steps << std::endl;

    return 0;
}
