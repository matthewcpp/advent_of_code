#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "02.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "02.txt";

constexpr int MIN_DELTA = 1;
constexpr int MAX_DELTA = 3;

bool check_levels(const std::vector<int>& levels){
    const bool increasing = levels[0] < levels[1];
    for (size_t i = 1; i < levels.size(); i++) {
        const int delta = std::abs(levels[i] - levels[i -1]);
        if (!(delta >= MIN_DELTA && delta <= MAX_DELTA && levels[i - 1] < levels[i] == increasing)) {
            return false;
        }
    }

    return true;
};

int main (int argc, char** argv) {
    std::string line;
    std::ifstream input(input_file);
    std::istringstream istring;
    std::vector<int> levels, levels_mod;
    int32_t count = 0;

    while(std::getline(input, line)) {
        istring.str(line);
        istring.clear();
        levels.clear();

        while (!istring.eof()) {
            istring >> levels.emplace_back();
        }

        if (check_levels(levels)) {
            count += 1;
        } else {
            for (size_t i = 0; i < levels.size(); i++) {
                levels_mod = levels;
                levels_mod.erase(levels_mod.begin() + i);

                if (check_levels(levels_mod)) {
                    count += 1;
                    break;
                }
            }
        }
    }

    std::cout << count << std::endl;
    return 0;
}