#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define DISPLAY_WIDTH 40

struct VM {
    int reg_x = 1;
    uint32_t cycle = 0;
    std::vector<std::string> scanlines;
    int scan_pos = 0;

    VM() {
        newScanLine();
    }

    void newScanLine() {
        scanlines.emplace_back(DISPLAY_WIDTH, '.');
        scan_pos = 0;
    }

    void incCycle() {
        cycle += 1;

        if (scan_pos >= DISPLAY_WIDTH) {
            newScanLine();
        }

        if (scan_pos >= reg_x - 1 && scan_pos <= reg_x + 1) {
            scanlines.back()[scan_pos] = '#';
        }

        scan_pos++;
    }

    void noop() {
        incCycle();
    }

    void add(int val) {
        incCycle();
        incCycle();
        reg_x += val;
    }

    void runProgram(std::string const & path) {
        std::ifstream input_file(path);
        std::string opcode;
        int value;

        for (;;) {
            input_file >> opcode;

            if (!input_file)
                break;

            if (opcode == "addx") {
                input_file >> value;
                add(value);
            }
            else if (opcode == "noop") {
                noop();
            }
        }
    }
};

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "10.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "10.txt";

    VM vm;
    vm.runProgram(input_file_path);

    for (auto const & scanline : vm.scanlines) {
        std::cout << scanline << std::endl;
    }

    return 0;
}
