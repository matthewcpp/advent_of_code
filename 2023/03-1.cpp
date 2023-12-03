#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "03.test.reddit.txt";
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

    for (const auto & symbol : symbols) {
        for (int row = std::max(symbol.y - 1, 0); row < std::min(symbol.y + 2, (int)parts.size()); row++) {
            auto& part_vec = parts[row];
            size_t p = 0;
            while (p < part_vec.size()) {
                auto & part = part_vec[p];
                
                if (symbol.x >= part.x - 1 && symbol.x <= part.x + part.width) {
                    sum += part.id;
                    std::swap(part, part_vec.back());
                    part_vec.pop_back();
                }
                else {
                    p += 1;
                }
            }
        }
    }

    std::cout << sum << std::endl;

    return 0;
}