#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>

struct VM {
    int reg_x = 1;
    uint32_t cycle = 0;
    std::function<void(VM*)> callback;
    std::unordered_set<uint32_t> breakpoints;

    void incCycle() {
        cycle += 1;
        if (breakpoints.count(cycle))
            callback(this);
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

    size_t total_signal_stength = 0;
    VM vm;
    vm.callback = [&total_signal_stength](VM* virtual_machine) {
        total_signal_stength += virtual_machine->reg_x * static_cast<int32_t>(virtual_machine->cycle);
    };
    vm.breakpoints = {20, 60, 100, 140, 180, 220};

    vm.runProgram(input_file_path);

    std::cout << total_signal_stength << std::endl;

    return 0;
}
