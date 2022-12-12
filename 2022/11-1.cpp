#include <cstdint>
#include <iostream>
#include <string>

#include "11.hpp"

#define ROUND_COUNT 20

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "11.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "11.txt";

    MonkeyParser parser;
    auto monkeys = parser.parseFile(input_file_path);

    for (int round = 0; round < ROUND_COUNT; round++) {
        for (auto& monkey : monkeys) {
            size_t item_count = monkey->items.size();

            for (size_t item = 0; item < item_count; item++) {
                auto worry_amount = monkey->operation.apply(monkey->items.front()) / 3;

                monkey->items.pop_front();
                monkey->items_inspected += 1;

                uint64_t monkey_dest = worry_amount % monkey->test.divisor == 0 ? monkey->test.true_dest : monkey->test.false_dest;
                monkeys[monkey_dest]->items.push_back(worry_amount);
            }
        }
    }

    std::sort(monkeys.begin(), monkeys.end(), [](std::unique_ptr<Monkey> const & a, std::unique_ptr<Monkey> const & b){
        return a->items_inspected > b->items_inspected;
    });

    auto monkey_business = monkeys[0]->items_inspected * monkeys[1]->items_inspected;

    std::cout << monkey_business << std::endl;

    return 0;
}
