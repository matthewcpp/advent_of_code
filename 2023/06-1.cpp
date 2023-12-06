#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "06.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "06.txt";

int main(int argc, char** argv) {
    std::ifstream file(input_file_path);
    
    auto read_values = [&file]() {
        std::vector<uint64_t> values;
        std::string line;
        uint64_t val;
        std::getline(file, line);
        std::istringstream str(line);
        str.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        while (str >> val) {
            values.push_back(val);
        }

        return values;
    };

    std::vector<uint64_t> results, race_times = read_values(), distance_records = read_values();

    for(size_t i = 0; i < race_times.size(); i++) {
        uint64_t min_hold = std::numeric_limits<uint64_t>::max();
        uint64_t max_hold = std::numeric_limits<uint64_t>::min();
        for (uint64_t hold_time = 0; hold_time < race_times[i]; hold_time++) {
            uint64_t d = hold_time * (race_times[i] - hold_time);

            if (d <= distance_records[i]) {
                continue;
            }

            min_hold = std::min(min_hold, hold_time);
            max_hold = std::max(max_hold, hold_time);
        }

        results.push_back(max_hold - min_hold + 1);
    }
    
    std::cout << std::accumulate(results.begin(), results.end(), 1, std::multiplies<uint64_t>()) << std::endl;;

    return 0;
}