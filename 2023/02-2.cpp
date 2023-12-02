#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "02.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "02.txt";

const std::array<std::string, 3> indices = {"red", "green", "blue"};

int main(int argc, char** argv)
{
    std::ifstream file(input_file_path);

    std::string line, result_color;
    uint64_t sum = 0, result_count;

    while (std::getline(file, line)) {
        std::array<uint64_t, 3> cube_counts = {0, 0, 0};
        std::istringstream stream(std::move(line));
        stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');

        while (stream >> result_count >> result_color) {
            if (result_color.back() == ';' || result_color.back() == ',') {
                result_color.pop_back();
            }

            auto cube_index = std::distance(indices.begin(), std::find(indices.begin(), indices.end(), result_color));
            cube_counts[cube_index] = std::max(result_count, cube_counts[cube_index]);
        }

        sum += std::accumulate(cube_counts.begin(), cube_counts.end(), 1, std::multiplies<uint64_t>());
    }

    std::cout << sum << std::endl;

    return 0;
}