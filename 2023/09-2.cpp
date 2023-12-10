#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "09.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "09.txt";


int main(int argc, char** argv) {
    std::ifstream file(input_file_path);
    std::string line;
    int64_t sum = 0;

    while (std::getline(file, line)) {
        std::istringstream str(std::move(line));
        int64_t val;

        std::vector<std::deque<int64_t>> iterations;
        auto& initial = iterations.emplace_back();

        while (str >> val) {
            initial.push_back(val);
        }

        bool all_zeros = false;
        while (!all_zeros) {
            auto& current = iterations.emplace_back();
            const auto& previous = iterations[iterations.size() - 2];

            for (size_t i = 1; i < previous.size(); i++) {
                current.push_back(previous[i] - previous[i - 1]);
            }

            all_zeros = std::find_if(current.begin(), current.end(), [](int64_t i) {return i != 0;}) == current.end();
        }
        
        for (int64_t i = iterations.size() - 2; i >= 0; i--) {
            auto& current = iterations[i];
            const auto& prev = iterations[i + 1];
            val = -prev.front() + current.front();
            current.push_front(val);
        }

        sum += iterations.front().front();
    }

    std::cout << sum << std::endl;
}