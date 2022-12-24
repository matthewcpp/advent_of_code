#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "06.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "06.txt";

    std::ifstream input_file(input_file_path);
    std::string input_str;
    while (std::getline(input_file, input_str)) {
        int start = 0, end = 1;

        while (end < input_str.length()) {
            for (int i = end - 1; i >= start; i--) {
                if (input_str[i] == input_str[end]) {
                    start = i + 1;
                    break;
                }
            }

            if (end - start == 13){
                std::cout << end + 1 << '(' << input_str.substr(start, 14) << ')' << std::endl;
                break;
            }

            end += 1;
        }
    }

    return 0;
}


