#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

uint32_t to_priority(char item);

int main(int argc, char** argv) {
    // auto const input_file_path = std::string(INPUT_DIRECTORY) + "3.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "3.txt";

    std::ifstream input_file(input_file_path);
    std::unordered_set<char> compartment;
    std::string current_line;

    uint32_t total_priority = 0;
    
    while (std::getline(input_file, current_line)) {
        for (size_t i  = 0; i < current_line.length() / 2; i++) {
            compartment.insert(current_line[i]);
        }

        for (size_t i = current_line.length() / 2; i < current_line.length(); i++) {
            char item = current_line[i];
            if (compartment.count(item)) {
                total_priority += to_priority(item);
                break;
            }
        }

        compartment.clear();
    }

    std::cout << total_priority << std::endl;

    return 0;
}

uint32_t to_priority(char item) {
    if (item >= 'a' && item <= 'z')
        return static_cast<uint32_t>(item - 'a') + 1;
    else if (item >= 'A' && item <= 'Z')
        return static_cast<uint32_t>(item - 'A') + 27;
    else
        return 0; // error?
}
