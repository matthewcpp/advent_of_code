#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#define HASH(y,x) (((y) << 32) | (x & 0xFFFFFFFF))

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.txt";

    std::vector<std::string> grid;
    std::string current_line;
    std::ifstream input_file(input_file_path);
    std::unordered_set<size_t> visible_spaces;

    while (std::getline(input_file, current_line))
        grid.push_back(current_line);
    
    for (size_t y = 0; y < grid.size(); y++) {
        char left_height = 0, right_height = 0;

        for (size_t x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] > left_height) {
                visible_spaces.insert(HASH(y, x));
                left_height = grid[y][x];
            }

            size_t right_index = grid[y].size() - 1 - x;
            if (grid[y][right_index] > right_height) {
                visible_spaces.insert(HASH(y, right_index));
                right_height = grid[y][right_index];
            }
        }
    }

    for (size_t x = 0; x < grid[0].size(); x++) {
        char top_height = 0, bottom_height = 0;

        for (size_t y = 0; y < grid.size(); y++) {
            if (grid[y][x] > top_height) {
                visible_spaces.insert(HASH(y, x));
                top_height = grid[y][x];
            }

            size_t bottom_index = grid.size() - 1 - y;
            if (grid[bottom_index][x] > bottom_height) {
                visible_spaces.insert(HASH(bottom_index, x));
                bottom_height = grid[bottom_index][x];
            }
        }
    }

    std::cout << visible_spaces.size() << std::endl;

    return 0;
}