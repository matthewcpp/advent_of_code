#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "01-test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "01.txt";

int main(int argc, char** argv) {
    std::unordered_map<uint32_t, uint32_t> left, right;
    uint32_t l, r;
    std::ifstream input(input_file);
    std::string line;
    std::istringstream str_stream;

    while (std::getline(input, line)) {
        str_stream.str(line);
        str_stream.clear();

        str_stream >> l >> r;
        left[l] += 1;
        right[r] += 1;
    }

    uint32_t score = 0;
    for (const auto& l : left) {
        auto result = right.find(l.first);
        if (result != right.end()) {
            score += l.first * l.second * result->second;
        }
    }

    std::cout << score << std::endl;

    return 0;
}