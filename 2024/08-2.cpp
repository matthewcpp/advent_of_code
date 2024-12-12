#include "vec2.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

//const std::string input_file = std::string(INPUT_DIRECTORY) + "08.test.txt";
const std::string input_file = std::string(INPUT_DIRECTORY) + "08.txt";

int main(int argc, char** argv) {
    std::string line;
    std::ifstream input(input_file);
    std::unordered_map<char, std::vector<IVec2>> locations;
    std::unordered_set<IVec2> antinodes;

    IVec2::Type width = 0, height = 0;
    while (std::getline(input, line)) {
        width = static_cast< IVec2::Type>(line.size());
        for (size_t x = 0; x < line.size(); x++) {
            if (line[x] == '.') {
                continue;
            }

            locations[line[x]].emplace_back(x,height);
        }
        height++;
    }

    for (const auto& loc : locations) {
        const auto& antennas = loc.second;
        for (size_t i = 0; i < antennas.size(); i++) {
            for (size_t j = 0; j < antennas.size(); j++) {
                if (i == j) {
                    continue;
                }

                const auto delta = antennas[j] - antennas[i];
                auto target =  antennas[j] + delta;
                antinodes.emplace(antennas[j]);

                while ( target.x >= 0 && target.x < width &&
                        target.y >= 0 && target.y < height) 
                {
                    antinodes.emplace(target);
                    target += delta;
                }
            }
        }
    }

    std::cout << antinodes.size() << std::endl;
}