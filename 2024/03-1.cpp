#include <iostream>
#include <fstream>
#include <regex>
#include <string>

const std::string input_file = std::string(INPUT_DIRECTORY) + "03.test.txt";
// const std::string input_file = std::string(INPUT_DIRECTORY) + "03.txt";

int main(int argc, char** argv) {
    std::ifstream input(input_file);
    std::string corrupted_memory;

    std::regex re(R"(mul\((\d{1,3}),(\d{1,3})\))");
    unsigned long total = 0;
    while (std::getline(input, corrupted_memory)) {
        for (std::smatch sm; std::regex_search(corrupted_memory, sm, re);) {
            total += std::stoul(sm[1]) * std::stoul(sm[2]);
            corrupted_memory = sm.suffix();
        }
    }

    std::cout << total << std::endl;

    return 0;
}