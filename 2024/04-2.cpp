#include "mlarocca/zip.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "04.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "04.txt";

const std::vector< std::vector<std::pair<int, int>>> search_dirs = {
    { {-1, -1}, {1, 1}, },
    { {1, 1}, {-1, -1},},
    { {-1, 1}, {1, -1}, },
    { {1, -1}, {-1, 1}, }
};

const std::vector<char> search_chars = {'M', 'S'};

int main(int argc, char** argv) {
    std::string line;
    std::vector<std::string> wordsearch;
    std::ifstream input(input_file);

    while (std::getline(input, line)) {
        wordsearch.push_back(line);
    }

    const int width = static_cast<int>(wordsearch[0].size()), height = static_cast<int>(wordsearch.size());
    int total = 0;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            if (wordsearch[h][w] != 'A') {
                continue;
            }

            int count = 0;
            for (const auto& search_dir : search_dirs) {
                count += [&wordsearch, &search_dir, &width, &height](int w, int h) {
                    for (const auto [offset, letter] : mlarocca::zip(search_dir, search_chars)) {
                        const int x = w + offset.first;
                        const int y = h + offset.second;

                        if (x < 0 || x >= width || y < 0 || y >= height || wordsearch[y][x] != letter) {
                            return 0;
                        }
                    }
                    return 1;
                }(w, h);
            }

            total += count >= 2 ? 1 : 0;
        }
    }

    std::cout << total << std::endl;
}