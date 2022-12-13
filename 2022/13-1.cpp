#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include "13.hpp"

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "13.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "13.txt";

    std::ifstream input_file(input_file_path);

    std::string left_packet_str, right_packet_str, line_break;
    size_t i = 1;
    size_t sum = 0;

    for (;;) {
        std::getline(input_file, left_packet_str);
        std::getline(input_file, right_packet_str);
        if (!input_file)
            break;

        std::getline(input_file, line_break);

        Packet left(left_packet_str);
        Packet right(right_packet_str);

        if (Packet::compare(left, right) == Packet::CompareResult::InOrder) {
            sum += i;
        }

        i++;
    }

    std::cout << sum << std::endl;
}
