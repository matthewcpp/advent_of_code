#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "02.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "02.txt";

const std::unordered_map<std::string, int> cube_counts = {
    {"red", 12}, {"green", 13}, {"blue", 14}
};

int main(int argc, char** argv)
{
    std::ifstream file(input_file_path);

    std::string line, result_color;
    uint64_t sum = 0, result_count;

    while (std::getline(file, line)) {
        size_t game_start = line.find(':');
        int game_id = std::stoi(line.substr(5, game_start));
        bool is_possible = true;

        std::istringstream stream(std::move(line));
        stream.ignore(game_start + 1);

        while (stream >> result_count >> result_color) {
            if (result_color.back() == ';' || result_color.back() == ',') {
                result_color.pop_back();
            }

            if (result_count > cube_counts.find(result_color)->second) {
                is_possible = false;
                break;
            }
        }

        if (is_possible) {
            sum += game_id;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}