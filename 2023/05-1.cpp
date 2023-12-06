#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "05.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "05.txt";

struct Range {
    Range(uint64_t dest_start, uint64_t source_start, uint64_t length) : dest_start(dest_start), source_start(source_start), length(length){}
    uint64_t dest_start, source_start, length; 
};

int main(int argc, char** argv) {
    std::ifstream file(input_file_path);
    std::string line_str;
    uint64_t seed_id, dest_start, source_start, length;

    std::vector<uint64_t> seed_list;
    std::vector<std::vector<Range>> range_maps;

    {
        std::getline(file, line_str);
        std::istringstream line(std::move(line_str));
        line.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        while (line >> seed_id) {
            seed_list.push_back(seed_id);
        }
    }

    while (std::getline(file, line_str)) {
        if (line_str.empty()) {
            range_maps.emplace_back();
            std::getline(file, line_str);
        }
        else {
            std::istringstream line(std::move(line_str));
            line >> dest_start >> source_start >> length;
            range_maps.back().emplace_back(dest_start, source_start, length);
        }
    }

    uint64_t lowest_location = std::numeric_limits<uint64_t>::max();

    for (auto value : seed_list) {
        for (const auto ranges : range_maps) {
            for (const auto range : ranges) {
                if (value >= range.source_start && value < range.source_start + range.length) {
                    value = range.dest_start + (value - range.source_start);
                    break;
                }
            }
        }

        lowest_location = std::min(lowest_location, value);
    }

    std::cout << lowest_location << std::endl;

    return 0;
}