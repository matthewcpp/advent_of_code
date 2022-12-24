#include <deque>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

int main (int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "05.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "05.txt";

    std::ifstream input_file(input_file_path);

    std::string current_line;

    // initialize stacks
    std::vector<std::deque<char>> stacks;
    std::getline(input_file, current_line);
    stacks.resize((current_line.length() + 1) / 4);
    input_file.seekg(0, std::ios::beg);
    
    while (std::getline(input_file, current_line)) {
        // did we finish parsing diagram ?
        if (std::isdigit(current_line[1])) {
            break;
        }

        // read the stack
        for (size_t i = 1; i < current_line.length(); i += 4) {
            if (current_line[i] != ' ') {
                size_t bucket = (i - 1) / 4;
                stacks[bucket].push_back(current_line[i]);
            }
        }
    }

    // eat the newline
    std::getline(input_file, current_line);

    //process move instructions
    std::regex instruction_regex("move (\\d+) from (\\d+) to (\\d+)");
    std::smatch results;

    while (std::getline(input_file, current_line)) {
        std::regex_match(current_line, results, instruction_regex);

        uint32_t count = std::stoul(results[1].str());

        auto & source_stack = stacks[std::stoul(results[2].str()) - 1];
        auto & dest_stack = stacks[std::stoul(results[3].str()) - 1];

        for (uint32_t i = 0; i < count; i++) {
            dest_stack.push_front(source_stack.front());
            source_stack.pop_front();
        }
    }

    // print the top of each item
    for (size_t i = 0 ; i < stacks.size(); i++) {
        auto const & stack = stacks[i];
        if (stack.empty())
            std::cout << "?";
        else
            std::cout << stacks[i].front();
    }

    std::cout << std::endl;

    return 0;
}