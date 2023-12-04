#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <unordered_set>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "04.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "04.txt";

int main(int argc, char** argv) {
    std::string line;
    std::ifstream file(input_file_path);

    std::unordered_set<int> winning_numbers;
    std::deque<int> bonus_scratchers;

    int total_scratchers = 0;

    while (std::getline(file, line)) {
        int copies = 1;
        if (!bonus_scratchers.empty()) {
            copies += bonus_scratchers.front();
            bonus_scratchers.pop_front();
        }

        std::istringstream str(std::move(line));
        str.ignore(std::numeric_limits<std::streamsize>::max(), ':');

        int number, num_matches = 0;
        while(str >> number) {
            winning_numbers.insert(number);
        }

        str.clear();
        str.ignore(std::numeric_limits<std::streamsize>::max(), '|');

        while(str >> number) {
            if (winning_numbers.count(number)) {
                num_matches += 1;
            }
        }

        bonus_scratchers.resize(std::max(static_cast<size_t>(num_matches), bonus_scratchers.size()), 0);
        for (int i = 0; i < num_matches; i++) {
            bonus_scratchers[i] += copies;
        }

        total_scratchers += copies;
        winning_numbers.clear();
    }

    std::cout << total_scratchers << std::endl;
}