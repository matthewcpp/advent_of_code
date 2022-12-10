#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

#include "9.hpp"

void adjust_tail(Vec2 const & head, Vec2 & tail);

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "9.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "9.txt";

    std::ifstream input_file(input_file_path);
    std::unordered_set<uint64_t> visited_locations;

    char direction;
    int distance;

    Vec2 head_pos = {0,0}, tail_pos = {0,0}, direction_vec;
    visited_locations.insert(tail_pos.hash());

    for (;;) {
        input_file >> direction >> distance;

        if (!input_file)
            break;

        direction_vec.setDirection(direction);

        for (int step = 0; step < distance; step++) {
            head_pos += direction_vec;
            adjust_tail(head_pos, tail_pos);
            visited_locations.insert(tail_pos.hash());
        }
    }

    std::cout << visited_locations.size();

    return 0;
}

void adjust_tail(Vec2 const & head, Vec2 & tail) {
    auto delta_vec = head - tail;
    auto abs_delta = Vec2::abs(delta_vec);

    if (abs_delta.x <= 1 && abs_delta.y <= 1)
        return;

    delta_vec.x = std::min(std::max(delta_vec.x, -1), 1);
    delta_vec.y = std::min(std::max(delta_vec.y, -1), 1);
    tail += delta_vec;
}
