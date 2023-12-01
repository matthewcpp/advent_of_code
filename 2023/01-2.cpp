#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "01-2.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "01.txt";

const std::array<std::string, 10> digits =
{
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

using DigitInfo = std::pair<uint64_t, uint64_t>;

std::vector<DigitInfo> get_digits(const std::string& line)
{
    std::vector<DigitInfo> results;

    for (size_t ch = 0; ch < line.size(); ch++) {
        if (std::isdigit(line[ch])) {
            results.emplace_back(std::make_pair(ch, static_cast<uint64_t>(line[ch] - '0')));
        }
    }

    for (size_t i = 1; i < digits.size(); i++) {
        uint64_t pos = line.find(digits[i]);
        while(pos != std::string::npos) {
            results.emplace_back(std::make_pair(pos, i));
            pos = line.find(digits[i], pos + digits[i].size());
        }
    }

    std::sort(results.begin(), results.end(), [](const DigitInfo& a, const DigitInfo& b) {
        return a.first < b.first;
    });

    return results;
}

int main (int argc, char** argv)
{
    uint64_t sum = 0;

    std::ifstream file(input_file_path);
    std::string line;

    while (std::getline(file, line)) {
        const auto results = get_digits(line);

        if (results.empty()) {
            continue;
        }

        sum += results.front().second * 10 + results.back().second;
    }

    std::cout << sum << std::endl;

    return 0;
}
