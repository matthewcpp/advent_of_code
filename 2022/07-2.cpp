#include <iostream>
#include <limits>

#include "07.hpp"

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "07.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "07.txt";

    Parser parser(input_file_path);
    parser.parseInput();

    const size_t DRIVE_SIZE = 70000000;
    const size_t UPDATE_REQUIRED_SIZE = 30000000;
    size_t needed_size = parser.rootDir()->totalSize() - (DRIVE_SIZE - UPDATE_REQUIRED_SIZE);
    size_t min_delete_size = std::numeric_limits<size_t>::max();
    size_t dir_delete_size;

    for (auto const & item : parser.dir_map) {
        //auto dir_size = get_total_dir_size(item.second.get());
        auto dir_size = item.second->totalSize();

        if (dir_size < needed_size)
            continue;

        size_t diff = dir_size - needed_size;
        if (diff < min_delete_size) {
            min_delete_size = diff;
            dir_delete_size = dir_size;
        }
    }

    std::cout << dir_delete_size << std::endl;

    return 0;
}
