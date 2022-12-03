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
    if (!input_file)
        return 0;

    std::vector<uint32_t> values;
    uint32_t current_value = 0;

    std::string value_str;
    while (std::getline(input_file, value_str)) {
        if (value_str.empty()) {
            values.push_back(current_value);
            current_value = 0;
        }
        else {
            current_value += std::stoul(value_str);
        }
    }

    values.push_back(current_value);
    std::sort(values.begin(), values.end(), std::greater<uint32_t>());
    uint32_t result = values[0] + values[1] + values[2];

    std::cout << result << std::endl;

    return 0;
}
