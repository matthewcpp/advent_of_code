#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "08-2.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.txt";

struct Path {
    Path(uint64_t id, const std::string& start)
        :start(start), current(start) {}
    std::string start, current;
    uint64_t endpoint_freq = std::numeric_limits<uint64_t>::max();
    inline bool freqIsSet() const { return endpoint_freq < std::numeric_limits<uint64_t>::max(); }
};

int main(int argc, const char** argv) {
    std::ifstream file(input_file_path);
    std::string line, directions;
    std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;
    std::vector<Path> paths;

    std::getline(file, directions);
    std::getline(file, line);

    std::regex node_regex("(\\w+) = \\((\\w+), (\\w+)\\)");
    std::smatch results;
    
    while (std::getline(file, line)) {
        std::regex_match(line, results, node_regex);
        nodes[results[1]] = std::make_pair(results[2], results[3]);

        if (results[1].str().back() == 'A') {
            paths.emplace_back(paths.size(), results[1]);
        }
    }

    bool all_paths_ready = false;
    uint64_t direction_index = 0, step_time = 0;
    while(!all_paths_ready) {
        step_time += 1;

        for (size_t i = 0; i < paths.size(); i++) {
            auto& path = paths[i];
            const auto result = nodes.find(path.current);
            path.current = directions[direction_index] == 'L' ? result->second.first : result->second.second;
            if (path.current.back() == 'Z' && !path.freqIsSet()) {
                path.endpoint_freq = step_time;
                all_paths_ready = std::find_if(paths.begin(), paths.end(), [](const Path& p) {return !p.freqIsSet();}) == paths.end();
            }
        }

        direction_index = ++direction_index % directions.length();
    }

    uint64_t step_count = paths[0].endpoint_freq;
    for (size_t i = 1; i < paths.size(); i++) {
        step_count = std::lcm(step_count, paths[i].endpoint_freq);
    }

    std::cout << step_count << std::endl;
}