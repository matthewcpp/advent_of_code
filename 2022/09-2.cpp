#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "09.hpp"

#define ROPE_LENGTH 10
#define ROPE_TAIL ROPE_LENGTH - 1

void update_rope(std::array<Vec2, ROPE_LENGTH> & rope, Vec2 const & direction);

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "09.test2.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "09.txt";

    std::ifstream input_file(input_file_path);
    std::unordered_set<uint64_t> visited_locations;

    char direction;
    int distance;

    Vec2 direction_vec;
    std::array<Vec2, ROPE_LENGTH> rope;
    for (auto & knot : rope) {
        knot = {0, 0};
    }

    visited_locations.insert(rope[ROPE_TAIL].hash());

    for (;;) {
        input_file >> direction >> distance;

        if (!input_file)
            break;

            direction_vec.setDirection(direction);

            for (int step = 0; step < distance; step++) {
                update_rope(rope, direction_vec);
                visited_locations.insert(rope[ROPE_TAIL].hash());
            }
    }

    std::cout << visited_locations.size() << std::endl;

    return 0;
}

void update_rope(std::array<Vec2, ROPE_LENGTH> & rope, Vec2 const & direction) {
    rope[0] += direction;

    for (size_t i = 1; i < ROPE_LENGTH; i++) {
        auto const & previous_knot = rope[i - 1];
        auto & current_knot = rope[i];

        auto delta_vec = previous_knot - current_knot;
        auto abs_delta = Vec2::abs(delta_vec);

        if (abs_delta.x <= 1 && abs_delta.y <= 1)
            return;

        delta_vec.x = std::min(std::max(delta_vec.x, -1), 1);
        delta_vec.y = std::min(std::max(delta_vec.y, -1), 1);
        current_knot += delta_vec;
    }
}
