#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "06.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "06.txt";

int64_t search(int64_t step, int64_t start, int64_t end, int64_t race_time, int64_t distance_record) {
    int64_t iterations = std::abs((end - start) / step);
    for (int64_t i = 1; i <= iterations; i++) {
        int64_t t = start + (i * step);
        int64_t d = t * (race_time - t);

        if (d > distance_record) {
            return t;
        }
    }

    return end;
}

int main(int argc, char** argv) {
    std::ifstream file(input_file_path);

    auto get_u64_line = [&file]() {
        std::string line;
        file.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        std::getline(file, line);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        return std::stoll(line);
    };

    const int64_t race_time = get_u64_line(), distance_record = get_u64_line();
    const int64_t starting_granularity = std::pow(10U, std::to_string(race_time).length()-1);

    int64_t low_bound = race_time;
    for (int64_t granularity = starting_granularity; granularity != 0; granularity /= 10) {
        low_bound = search(granularity, 0, low_bound, race_time, distance_record);
    }

    int64_t high_bound = 0;
    for (int64_t granularity = starting_granularity; granularity != 0; granularity /= 10) {
        high_bound = search(-granularity, race_time, high_bound, race_time, distance_record);
    }

    std::cout << high_bound - low_bound + 1 << std::endl;

    return 0;
}