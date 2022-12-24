#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

uint32_t calc_senic_score(std::vector<std::vector<uint8_t>> const & grid, int x, int y);

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "08.txt";

    std::vector<std::vector<uint8_t>> grid;
    std::string current_line;
    std::ifstream input_file(input_file_path);

    while (std::getline(input_file, current_line)) {
        grid.emplace_back();
        auto & row = grid.back();

        for (auto ascii : current_line) {
            row.push_back(ascii - '0');
        }
    }
    uint32_t max_secnic_score = 0;

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x ++) {
            max_secnic_score = std::max(max_secnic_score, calc_senic_score(grid, x, y));
        }
    }

    std::cout << max_secnic_score << std::endl;

    return 0;
}

uint32_t calc_senic_score(std::vector<std::vector<uint8_t>> const & grid, int x, int y) {
    uint32_t left_score = 0, right_score = 0, top_score = 0, bottom_score = 0;

    for (int left = x - 1; left >= 0; left--) {
        left_score += 1;
        if (grid[y][left] >= grid[y][x])
            break;
    }

    for (int right = x + 1; right < grid[y].size(); right++) {
        right_score += 1;
        if (grid[y][right] >= grid[y][x])
            break;
    }

    for (int top = y - 1; top >= 0; top--) {
        top_score += 1;
        if (grid[top][x] >= grid[y][x])
            break;
    }

    for (int bottom = y + 1; bottom < grid.size(); bottom++) {
        bottom_score += 1;
        if (grid[bottom][x] >= grid[y][x])
            break;
    }

    return left_score * right_score * top_score * bottom_score;
}
