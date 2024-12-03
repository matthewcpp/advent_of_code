#include <iostream>
#include <fstream>
#include <regex>
#include <string>

const std::string input_file = std::string(INPUT_DIRECTORY) + "03.test2.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "03.txt";

int main(int argc, char** argv) {
    std::ifstream input(input_file);
    std::string corrupted_memory;

    std::regex re(R"((don't|do)\(\)|mul\((\d{1,3}),(\d{1,3})\))");
    unsigned long total = 0;
    bool mul_enabled = true;
    while (std::getline(input, corrupted_memory)) {
        for (std::smatch sm; std::regex_search(corrupted_memory, sm, re);) {
            const auto command = sm[0].str();

            if (command.find("mul") == 0 && mul_enabled) {
                total += std::stoul(sm[2]) * std::stoul(sm[3]);
            } else if (command.find("don't") == 0) {
                mul_enabled = false;
            } else if (command.find("do") == 0) {
                mul_enabled = true;
            }
            corrupted_memory = sm.suffix();
        }
    }

    std::cout << total << std::endl;

    return 0;
}