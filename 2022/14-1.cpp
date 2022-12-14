#include "14.hpp"

#include <iostream>

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "14.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "14.txt";

    Map map;
    map.parse(input_file_path, Point2(500, 0));

    size_t unit_count = 0;
    Point2 starting_point = map.toLocalPoint(map.world_start_pos);
    std::vector<Point2> sand_path;

    for (;;) {
        Point2 sand_pos = Simulate::sandUnitPosition(map, starting_point, sand_path);

        if (!map.isInBounds(sand_pos)) {
            break;
        }

        map.grid[sand_pos.y][sand_pos.x] = 'o';
        unit_count += 1;
    }

    for (size_t i  = 1; i < sand_path.size(); i++) {
        auto const & pt = sand_path[i];
        map.grid[pt.y][pt.x] = '~';
    }

    std::cout << unit_count << std::endl;

    return 0;
}