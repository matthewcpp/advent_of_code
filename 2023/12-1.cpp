#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

auto const input_file_path = std::string(INPUT_DIRECTORY) + "12.test.txt";
// auto const input_file_path = std::string(INPUT_DIRECTORY) + "12.txt";

int main(int argc, char** argv) {
    {
        std::ifstream file(input_file_path);
        std::string line;
        while (std::getline(file, line)) {

            std::string mask;
        }
    }
}