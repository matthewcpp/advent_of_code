#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main (int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "1.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "1.txt";

    std::ifstream input_file(input_file_path);
    uint32_t current_value = 0, max_value = 0;

    std::string value_str;
    while (std::getline(input_file, value_str)) {
        if (value_str.empty()) {
            max_value = std::max(max_value, current_value);
            current_value = 0;
        }
        else {
            current_value += std::stoul(value_str);
        }
    }

    max_value = std::max(max_value, current_value);

    std::cout << max_value << std::endl;

    return 0;
}
