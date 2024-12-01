#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "01-test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "01.txt";

int main(int argc, char** argv) {
    std::vector<int> left, right;
    std::ifstream input(input_file);
    std::string line;
    std::istringstream str_stream;

    while (std::getline(input, line)) {
        str_stream.str(line);
        str_stream.clear();

        str_stream >> left.emplace_back() >> right.emplace_back();
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    uint32_t sum = 0;

    for (size_t i = 0; i < left.size(); i++) {
        sum += std::abs(left[i] - right[i]);
    }

    std::cout << sum << std::endl;

    return 0;
}