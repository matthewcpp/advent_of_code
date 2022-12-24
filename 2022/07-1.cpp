#include <iostream>
#include "07.hpp"


int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "07.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "07.txt";

    Parser parser(input_file_path);
    parser.parseInput();

    const size_t MAX_VALUE = 100000;

    size_t total_val = 0;
    for (auto const & item : parser.dir_map) {
        auto dir_size = item.second->totalSize();
        if (dir_size <= MAX_VALUE)
            total_val += dir_size;
    }

    std::cout << total_val << std::endl;

    return 0;
}
