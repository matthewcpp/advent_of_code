#include "15.hpp"

#include <cstdint>
#include <iostream>

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "15.test.txt";
    //const int64_t target_y = 10;

    auto const input_file_path = std::string(INPUT_DIRECTORY) + "15.txt";
    const int64_t target_y = 2000000;

    Map map;
    map.parse(input_file_path);

    
    int64_t filled_positions = 0;

    for (int64_t x = map.min_pos.x; x <= map.max_pos.x; x++) {
        Point2 pt = {x, target_y};

        if (map.beacons.count(pt)) {
            continue;
        }

        for (auto const & sensor : map.sensors) {
            int64_t distance = Point2::manhattanDistance(sensor.position, pt);

            if (distance <= sensor.radius) {
                filled_positions += 1;
                break;
            }
        }
    }

    std::cout << filled_positions << std::endl;

    return 0;
}