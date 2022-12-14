#include "14.hpp"
#include <iostream>

void prepend_column(Map& map) {
    for (auto & row : map.grid) {
        row.push_front('.');
    }

    map.grid.back()[0] = '#';
    map.world_min.x -= 1;
}

void append_column(Map& map) {
    for (auto & row : map.grid) {
        row.push_back('.');
    }

    map.grid.back().back() = '#';
    map.world_max.x += 1;
}

void append_row(Map& map, char ch) {
    map.grid.emplace_back(map.grid[0].size(), ch);
    map.world_max.y += 1;
}

int main(int arc, char** argv) {
    // auto const input_file_path = std::string(INPUT_DIRECTORY) + "14.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "14.txt";

    Map map;
    map.parse(input_file_path, Point2(500, 0));

    append_row(map, '.');
    append_row(map, '#');

    size_t unit_count = 0;
    Point2 starting_point = map.toLocalPoint(map.world_start_pos);
    std::vector<Point2> sand_path;

    for (;;) {
        Point2 sand_pos = Simulate::sandUnitPosition(map, starting_point, sand_path);

        if (!map.isInBounds(sand_pos)) {
            if (sand_pos.x < 0) {
                prepend_column(map);
                sand_pos = {0, static_cast<int>(map.grid.size() -2)};
            }
            else {
                append_column(map);
                sand_pos = {static_cast<int>(map.grid.front().size() - 1), static_cast<int>(map.grid.size() -2)};
            }

            starting_point = map.toLocalPoint(map.world_start_pos);
        }

        map.grid[sand_pos.y][sand_pos.x] = 'o';
        unit_count += 1;

        if (sand_pos == starting_point)
            break;
    }

    std::cout << unit_count << std::endl;
    return 0;
}