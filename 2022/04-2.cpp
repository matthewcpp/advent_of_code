
#include <cstdint>
#include <fstream>
#include <iostream>
#include <utility>
#include <string>
#include <sstream>

bool overlaps(std::pair<uint32_t, uint32_t> const & a, std::pair<uint32_t, uint32_t> const & b);

int main (int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "04.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "04.txt";

    std::ifstream input_file(input_file_path);

    std::string current_line;
    char ch;
    uint32_t overlapping = 0;

    while(std::getline(input_file, current_line)) {
        std::pair<uint32_t, uint32_t> a, b;
        std::istringstream str_stream(current_line);
        str_stream >> a.first >> ch >> a.second >> ch >> b.first >> ch >> b.second;

        if (overlaps(a,b)) {
            overlapping += 1;
        }
    }

    std::cout << overlapping << std::endl;

    return 0;
}

bool overlaps(std::pair<uint32_t, uint32_t> const & a, std::pair<uint32_t, uint32_t> const & b) {
    if (a.first > b.second)
        return false;
    
    if (a.second < b.first)
        return false;

    return true;
}