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
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "03.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "03.txt";

    std::ifstream input_file(input_file_path);
    std::array<std::unordered_set<char>, 3> compartments;
    std::unordered_set<char> temp;
    std::string current_line;

    uint32_t total_priority = 0;
    uint32_t index = 0;
    
    while (std::getline(input_file, current_line)) {
        compartments[index++].insert(current_line.begin(), current_line.end());

        if (index == 3) {
            for (auto const item : compartments[0]) {
                if (compartments[1].count(item)) {
                    temp.insert(item);
                }
            }

            for (auto const item : temp) {
                if (compartments[2].count(item)) {
                    total_priority += to_priority(item);
                    break;
                }
            }

            for (int i = 0; i < 3; i++)
                compartments[i].clear();

            temp.clear();
            index = 0;
        }
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
