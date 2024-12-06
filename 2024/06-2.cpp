#include <vec2.hpp>

#include <algorithm>
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
    std::vector<IVec2> path;
    IVec2 start_pos;

    while (std::getline(input, line)) {
        auto start_pos_x = line.find('^');
        if (start_pos_x != std::string::npos) {
            start_pos = {static_cast<int>(start_pos_x), static_cast<int>(map.size())};
        }
        map.emplace_back(line);
    }
    const int width = static_cast<int>(map[0].size()), height = static_cast<int>(map.size());

    size_t possible_placements = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] != '.') {
                continue;
            }
            path.clear();
            path.emplace_back(start_pos);

            map[y][x] = '#';
            IVec2 pos = start_pos, dir = Up;
            for (;;) {
                IVec2 next_pos = pos + dir;
                if (next_pos.x < 0 || next_pos.x >= width || next_pos.y < 0 || next_pos.y >=height) {
                    break;
                }

                if (map[next_pos.y][next_pos.x] == '#') {
                    if (dir == Up) { dir = Right; }
                    else if (dir == Right) { dir = Down; }
                    else if (dir == Down) { dir = Left; }
                    else if (dir == Left) { dir = Up; }
                } else {
                    pos = next_pos;

                    bool loop_found = [&map, &path, &pos]() {
                        auto last_visit = std::find(path.rbegin(), path.rend(), pos);
                        if (last_visit != path.rend()) {
                            auto loop_size = std::distance(path.rbegin(), last_visit);
                            auto prev = ++last_visit;
                            auto current = path.rbegin();
                            if (loop_size <= std::distance(path.begin(), prev.base())) {
                                for (decltype(loop_size) i = 0; i < loop_size; i++, ++prev, ++current) {
                                    if (*prev != *current) {
                                        return false;
                                    }
                                }
                                return true;
                            }
                        }

                        return false;
                    }();

                    if (loop_found) {
                        possible_placements +=1; 
                        break; 
                    }

                    path.emplace_back(pos);

                }
            }
            map[y][x] = '.';
        }
    }

    std::cout << possible_placements << std::endl;

    return 0;
}