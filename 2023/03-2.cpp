#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "03.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "03.txt";

struct Symbol {
    Symbol(char ch, int x, int y) : ch(ch), x(x), y(y) {}
    char ch; int x, y;
};

struct Part {
    Part(int id, int x, int y, int width) : id(id), x(x), y(y), width(width) {}
    int id, x, y, width;
};

int main(int argc, char** argv) {
    std::vector<Symbol> symbols;
    std::vector<std::vector<Part>> parts;

    int x, y, sum = 0;
    std::ifstream file(input_file_path);
    std::string line;

    while (std::getline(file, line)) {
        auto & line_vec = parts.emplace_back();
        x = 0;
        while (x < line.length()) {
            if (line[x] == '.') {
                x += 1;
            }
            else if (std::isdigit(line[x])) {
                size_t end = x + 1;
                while (end < line.length() && std::isdigit(line[end])) {
                    end += 1;
                }
                int id;
                std::from_chars(line.data() + x, line.data() + end, id);
                line_vec.emplace_back(id, x, y, end - x);
                x = end;
            }
            else {
                symbols.emplace_back(line[x],x, y);
                x += 1;
            }
        }

        y += 1;
    }

    std::vector<const Part*> adjacent_parts;
    for (const auto & symbol : symbols) {
        if (symbol.ch != '*') {
            continue;
        }

        for (int row = std::max(symbol.y - 1, 0); row < std::min(symbol.y + 2, (int)parts.size()); row++) {
            for (const auto & part : parts[row]) {
                if (symbol.x >= part.x - 1 && symbol.x <= part.x + part.width) {
                    adjacent_parts.push_back(&part);
                }
            }
        }

        if (adjacent_parts.size() == 2) {
            sum += adjacent_parts[0]->id * adjacent_parts[1]->id;
        }

        adjacent_parts.clear();
    }

    std::cout << sum << std::endl;

    return 0;
}