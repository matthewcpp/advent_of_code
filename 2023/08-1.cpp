#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.test2.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.txt";

int main(int argc, const char** argv) {
    std::ifstream file(input_file_path);
    std::string line, directions;
    std::unordered_map<std::string, std::pair<std::string, std::string>> nodes;

    std::getline(file, directions);
    std::getline(file, line);

    std::regex node_regex("(\\w+) = \\((\\w+), (\\w+)\\)");
    std::smatch results;

    while (std::getline(file, line)) {
        std::regex_match(line, results, node_regex);
        nodes[results[1]] = std::make_pair(results[2], results[3]);
    }

    std::string current = nodes.find("AAA")->first;
    uint64_t direction_index = 0, step_count = 0;
    while (current != "ZZZ") {
        const auto result = nodes.find(current);
        current = directions[direction_index] == 'L' ? result->second.first : result->second.second;

        direction_index = ++direction_index % directions.length();
        step_count += 1;
    }

    std::cout << step_count << std::endl;

    return 0;
}