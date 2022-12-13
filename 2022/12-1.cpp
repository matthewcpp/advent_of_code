#include <iostream>

#include "12.hpp"

int main(int argc, char** argv) {
    // auto const input_file_path = std::string(INPUT_DIRECTORY) + "12.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "12.txt";

    Map map;
    map.load(input_file_path);
    Solver solver(map);
    bool result = solver.solve(map.start, map.exit);

    std::vector<Point2> path = solver.getPath();
    if (path.size() > 0)
        std::cout << path.size() - 1 << std::endl;
    else
        std::cout << "No path found" << std::endl;

    return 0;
}