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
    int sum = 0;

    while (std::getline(file, line)) {
        std::istringstream str(std::move(line));
        str.ignore(std::numeric_limits<std::streamsize>::max(), ':');

        int number, value = 0;
        while(str >> number) {
            winning_numbers.insert(number);
        }

        str.clear();
        str.ignore(std::numeric_limits<std::streamsize>::max(), '|');

        while(str >> number) {
            if (winning_numbers.count(number)) {
                value = value > 0 ? value * 2 : 1;
            }
        }

        sum += value;
        winning_numbers.clear();
    }

    std::cout << sum << std::endl;
}