#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "vec2.hpp"

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "11.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "11.txt";

//constexpr IVec2::Type ExpansionRate = 10;
//constexpr IVec2::Type ExpansionRate = 100;
constexpr IVec2::Type ExpansionRate = 1'000'000;

int main(int argc, char** argv) {
    std::vector<IVec2> galaxies;
    {
        std::ifstream file(input_file_path);
        std::string line;
        int y = 0;
        std::getline(file, line);
        std::vector<uint8_t> galaxy_columns(line.size(), 0);
        
        do {
            auto galaxy_pos = line.find('#');

            if (galaxy_pos == std::string::npos) {
                y += ExpansionRate;
                continue;
            }

            while (galaxy_pos != std::string::npos) {
                galaxies.emplace_back(static_cast<int>(galaxy_pos), y);
                galaxy_columns[galaxy_pos] = 1;
                galaxy_pos = line.find('#', galaxy_pos + 1);
            }

            y += 1;
        
        } while (std::getline(file, line));

        std::vector<IVec2::Type> galaxy_offsets(galaxy_columns.size(), 0);
        for (size_t i = 0; i <galaxy_columns.size(); i++) {
            if (galaxy_columns[i] != 0) {
                continue;
            }

            for (size_t j = i; j < galaxy_offsets.size(); j++) {
                galaxy_offsets[j] += ExpansionRate-1;
            }
        }

        for (auto& galaxy : galaxies) {
            galaxy.x += galaxy_offsets[galaxy.x];
        }
    }

    size_t total_distance = 0;
    for (size_t i = 0; i < galaxies.size(); i++) {
        for (size_t j = i + 1; j < galaxies.size(); j++) {
            total_distance += IVec2::ManhattanDistance(galaxies[i], galaxies[j]);
        }
    }

    std::cout << total_distance << std::endl;

    return 0;
}