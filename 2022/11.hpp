#pragma once

#include <algorithm>
#include <cstdint>
#include <deque>
#include <fstream>
#include <limits>
#include <memory>
#include <regex>
#include <sstream>
#include <vector>

struct Monkey {
    struct Test {
        uint64_t divisor;
        uint64_t true_dest;
        uint64_t false_dest;
    };

    struct Operation {
        enum Operator {
            Add = '+',
            Multiply = '*'
        };

        uint64_t apply(uint64_t old) {
            uint64_t op_val = use_value ? value : old;

            switch (op) {
                case Operator::Add:
                    return old + op_val;

                case Operator::Multiply:
                    return old * op_val;
            }
        }

        Operator op;
        uint64_t value;
        bool use_value = true;
    };

    std::deque<uint64_t> items;
    Operation operation;
    Test test;
    uint64_t items_inspected = 0;
};

class MonkeyParser {
public:
    std::vector<std::unique_ptr<Monkey>> parseFile(std::string const & path) {
        input_file.open(path);
        std::string current_line;
        std::vector<std::unique_ptr<Monkey>> monkeys;

        while (std::getline(input_file, current_line)) {
            auto monkey = std::make_unique<Monkey>();
            parseItemList(monkey->items);
            parseOperation(monkey->operation);
            parseTest(monkey->test);
            monkeys.push_back(std::move(monkey));
            std::getline(input_file, current_line);
        }

        return monkeys;
    }

private:
    void parseItemList(std::deque<uint64_t>& item_list) {
        std::string current_line, current_item;
        std::getline(input_file, current_line);
        std::istringstream item_list_str(current_line.substr(current_line.find_first_of(':') + 1));

        while(std::getline(item_list_str, current_item, ',')){
            item_list.push_back(std::stoull(current_item));
        }
    }

    void parseOperation(typename Monkey::Operation& operation) {
        std::string current_line;
        std::smatch results;

        std::getline(input_file, current_line);
        std::regex_match(current_line, results, operation_regex);

        operation.op = static_cast<Monkey::Operation::Operator>(results[1].str()[0]);
        if (results[2] == "old") {
            operation.use_value = false;
        }
        else {
            operation.value = std::stoul(results[2]);
            operation.use_value = true;
        }
    }

    void parseTest(typename Monkey::Test& test) {
        std::string current_line;
        std::smatch results;

        std::getline(input_file, current_line);
        std::regex_match(current_line, results, test_action_regex);
        test.divisor = std::stoll(results[1]);

        std::getline(input_file, current_line);
        std::regex_match(current_line, results, test_result_regex);
        test.true_dest = std::stoull(results[1]);

        std::getline(input_file, current_line);
        std::regex_match(current_line, results, test_result_regex);
        test.false_dest = std::stoull(results[1]);
    }

public:
    MonkeyParser() {
        operation_regex = "\\s*Operation: new = old ([\\+\\*]) (\\w+)";
        test_action_regex = "\\s*Test: divisible by (\\d+)";
        test_result_regex = "\\s*If \\w+: throw to monkey (\\d)";
    }

private:
    std::ifstream input_file;
    std::regex operation_regex;
    std::regex test_action_regex;
    std::regex test_result_regex;
};

class Simulation {
public:
    uint64_t run(std::string const & input_file_path, size_t round_count, bool panic_reduction) {
        MonkeyParser parser;
        auto monkeys = parser.parseFile(input_file_path);

        std::vector<uint64_t> test_ops;
        std::transform(monkeys.begin(), monkeys.end(), std::back_inserter(test_ops), [](std::unique_ptr<Monkey> const & m){
            return m->test.divisor;
        });

        //auto lcm = compute_lcm(test_ops);

        for (int round = 0; round < round_count; round++) {
            if (round > 0 && round % 100 == 0) {
                std::cout << "round " << round << std::endl;
            }
            for (auto& monkey : monkeys) {
                size_t item_count = monkey->items.size();

                for (size_t item = 0; item < item_count; item++) {
                    auto worry_amount = monkey->operation.apply(monkey->items.front());
                    if (panic_reduction)
                        worry_amount /= 3;

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

        return monkeys[0]->items_inspected * monkeys[1]->items_inspected;
    }

private:
    static uint64_t compute_lcm(std::vector<uint64_t> const & numbers) {
        std::vector<uint64_t> values = numbers;

        while ( !std::equal(values.begin() + 1, values.end(), values.begin()) ) {
            size_t min_index = std::distance(values.begin(), std::min_element(values.begin(), values.end()));
            values[min_index] += numbers[min_index];
        }

        return values[0];
    }

private:
    std::vector<std::unique_ptr<Monkey>> monkeys;
};
