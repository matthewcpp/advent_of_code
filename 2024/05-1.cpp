#include "split_str.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

const std::string input_file = std::string(INPUT_DIRECTORY) + "05.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "05.txt";

int main(int argc, char** argv) {
    std::string line;
    std::ifstream input(input_file);
    std::unordered_map<std::string, std::unordered_set<std::string>> dependencies;

    for (;;) {
        std::getline(input, line);
        if (line.empty()){
            break;
        }

        const auto split = split_str(line, '|');
        dependencies[split[1]].insert(split[0]);
    }

    uint64_t total = 0;

    while (std::getline(input, line)) {
        const auto updates = split_str(line, ',');
        bool is_correct = [&dependencies, &updates]() {
            for (size_t i = 0; i < updates.size(); i++) {
                const auto item_deps = dependencies.find(updates[i]);
                if (item_deps == dependencies.end()) {
                    continue;
                }

                for (size_t j = i + 1; j < updates.size(); j++) {
                    if (item_deps->second.count(updates[j])) {
                        return false;
                    }
                }
            }

            return true;
        }();

        total += is_correct ? std::stoul(updates[updates.size() / 2]) : 0;
    }

    std::cout << total << std::endl;

    return 0;
}