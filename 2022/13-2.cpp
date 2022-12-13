#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>


#include "13.hpp"

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "13.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "13.txt";

    std::vector<Packet> packets;

    std::ifstream input_file(input_file_path);

    std::string left_packet_str, right_packet_str, line_break;

    for (;;) {
        std::getline(input_file, left_packet_str);
        std::getline(input_file, right_packet_str);
        if (!input_file)
            break;

        std::getline(input_file, line_break);

        packets.emplace_back().parse(left_packet_str);
        packets.emplace_back().parse(right_packet_str);
    }

    Packet marker_a = packets.emplace_back().parse("[[2]]");
    Packet marker_b = packets.emplace_back().parse("[[6]]");

    std::sort(packets.begin(), packets.end(), [](Packet const & a, Packet const & b) {
        return Packet::compare(a,b) == Packet::CompareResult::InOrder;
    });

    auto marker_a_pos = std::find_if(packets.begin(), packets.end(), [&marker_a](Packet const & packet){
        return Packet::compare(packet, marker_a) == Packet::CompareResult::Equal;
    });

    auto marker_b_pos = std::find_if(packets.begin(), packets.end(), [&marker_b](Packet const & packet){
        return Packet::compare(packet, marker_b) == Packet::CompareResult::Equal;
    });

    size_t marker_a_index = std::distance(packets.begin(), marker_a_pos) + 1;
    size_t marker_b_index = std::distance(packets.begin(), marker_b_pos) + 1;

    std::cout << marker_a_index * marker_b_index << std::endl;
}
