#include "split_str.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

//const std::string input_file = std::string(INPUT_DIRECTORY) + "07.test.txt";
const std::string input_file = std::string(INPUT_DIRECTORY) + "07.txt";

int main(int argc, char** argv) {
    std::string line;
    std::ifstream input(input_file);
    uint64_t calibration_result = 0;
    while (std::getline(input, line)) {
        std::unordered_set<uint64_t> prev, current;

        const auto parts = split_str(line, ": ");
        const auto values = split_str(parts[1], ' ');
        const uint64_t target = std::stoull(parts[0]);
        current.insert(std::stoul(values[0]));

        for (size_t i = 1; i < values.size(); i++) {
            prev = std::move(current);

            const uint64_t val = std::stoul(values[i]);
            for (const auto& item : prev) {
                uint64_t v = item + val;
                if (v <= target) current.insert(v);
                v = item * val;
                if (v <= target) current.insert(v);
                v = std::stoull(std::to_string(item) + values[i]);
                if (v <= target) current.insert(v);
            }
        }

        if (current.count(target)) {
            calibration_result += target;
        }
    }

    std::cout << calibration_result << std::endl;

    return 0;
}