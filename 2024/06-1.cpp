#include <vec2.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "06.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "06.txt";

static const IVec2 Up = {0, -1};
static const IVec2 Down = {0, 1};
static const IVec2 Left = {-1, 0};
static const IVec2 Right = {1, 0};

int main (int argc, char** argv) {
    std::string line;
    std::ifstream input(input_file);
    std::vector<std::string> map;
    std::unordered_set<IVec2> distinct_positions;
    IVec2 pos, dir = {0, -1};

    while (std::getline(input, line)) {
        auto start_pos_x = line.find('^');
        if (start_pos_x != std::string::npos) {
            pos = {static_cast<int>(start_pos_x), static_cast<int>(map.size())};
            distinct_positions.insert(pos);
        }
        map.emplace_back(line);
    }

    for (;;) {
        map[pos.y][pos.x] = 'X';
        IVec2 next_pos = pos + dir;
        if (next_pos.x < 0 || next_pos.x >= map[0].size() || next_pos.y < 0 || next_pos.y >= map.size()) {
            break;
        }

        if (map[next_pos.y][next_pos.x] == '#') {
            if (dir == Up) { dir = Right; }
            else if (dir == Right) { dir = Down; }
            else if (dir == Down) { dir = Left; }
            else if (dir == Left) { dir = Up; }
        } else {
            pos = next_pos;
            distinct_positions.insert(pos);
        }
    }

    std::cout << distinct_positions.size() << std::endl;

    return 0;
}