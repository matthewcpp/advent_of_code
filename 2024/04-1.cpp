#include "mlarocca/zip.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "04.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "04.txt";

const std::vector< std::vector<std::pair<int, int>>> search_dirs = {
    { {1, 0}, {2, 0}, {3, 0} }, // forward
    { {-1, 0}, {-2, 0}, {-3, 0} }, // backward
    { {0, 1}, {0, 2}, {0, 3} }, // up
    { {0, -1}, {0, -2}, {0, -3} }, // down
    { {1, 1}, {2, 2}, {3, 3} }, // diagDR
    { {1, -1}, {2, -2}, {3, -3} }, // diagUR
    { {-1, 1}, {-2, 2}, {-3, 3} }, // diagDL
    { {-1, -1}, {-2, -2}, {-3, -3} }// diagUL
};

const std::vector<char> search_chars = {'M', 'A', 'S'};

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
            if (wordsearch[h][w] != 'X') {
                continue;
            }

            for (const auto& search_dir : search_dirs) {
                total += [&wordsearch, &search_dir, &width, &height](int w, int h) {
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
        }
    }

    std::cout << total << std::endl;
}