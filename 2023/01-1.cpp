#include <fstream>
#include <iostream>
#include <string>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "01.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "01.txt";

int main (int argc, char** argv)
{
    uint64_t sum = 0;

    std::ifstream file(input_file_path);
    std::string line;

    while (std::getline(file, line)) {
        uint64_t value = 0;

        for (auto it = line.begin(); it != line.end(); ++it) {
            if (std::isdigit(*it)) {
                value += static_cast<uint64_t>(*it - '0') * 10;
                break;
            }
        }

        for (auto it = line.rbegin(); it != line.rend(); ++it) {
            if (std::isdigit(*it)) {
                value += static_cast<uint64_t>(*it - '0');
                break;
            }
        }

        sum += value;
    }

    std::cout << sum << std::endl;
    return 0;
}
