#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

const std::string input_file = std::string(INPUT_DIRECTORY) + "02.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "02.txt";

constexpr int MIN_DELTA = 1;
constexpr int MAX_DELTA = 3;

int main (int argc, char** argv) {
    std::string line;
    std::ifstream input(input_file);
    std::istringstream istring;
    std::vector<int> levels;
    int32_t count = 0;

    while(std::getline(input, line)) {
        istring.str(line);
        istring.clear();
        levels.clear();

        while (!istring.eof()) {
            istring >> levels.emplace_back();
        }

        const bool level_is_safe = [&levels](){
            const bool increasing = levels[0] < levels[1];
            for (size_t i = 1; i < levels.size(); i++) {
                const int delta = std::abs(levels[i] - levels[i -1]);
                if (!(delta >= MIN_DELTA && delta <= MAX_DELTA && levels[i - 1] < levels[i] == increasing)) {
                    return false;
                }
            }

            return true;
        }();

        if (level_is_safe) {
            count += 1;
        }
    }

    std::cout << count << std::endl;
    return 0;
}