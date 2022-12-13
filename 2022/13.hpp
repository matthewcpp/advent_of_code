#pragma once

#include <any>
#include <cstdint>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct Packet {
    std::vector<std::any> values;

    Packet() {}
    Packet(std::string & str) {
        parse(str);
    }

    Packet(Packet const & p) {
        values = p.values;
    }

    Packet& operator=(Packet const & p) {
        values = p.values;
        return *this;
    }

    Packet(Packet&& p) {
        values = std::move(p.values);
    }

    Packet& operator=(Packet && p) {
        values = std::move(p.values);
        return *this;
    }

    ~Packet() = default;

    Packet& parse(std::string const & packet_str) {
        values.clear();

        std::stack<std::vector<std::any>*> list_stack;
        list_stack.push(&values);

        size_t i = 1;
        while (i < packet_str.size() - 1) {
            if (packet_str[i] == '['){
                auto& any_val = list_stack.top()->emplace_back();
                any_val = std::vector<std::any>();
                list_stack.push(std::any_cast<std::vector<std::any>>(&any_val));
                i++;
            }
            else if (std::isdigit(packet_str[i])) {
                size_t start = i;
                do {
                    i++;
                }while (std::isdigit(packet_str[i]) && i < packet_str.size());
                std::string val_string = packet_str.substr(start, i - start);
                int val = std::stoi(val_string);
                auto& any_val = list_stack.top()->emplace_back();
                any_val = val;
            }
            else if (packet_str[i] == ',') {
                i++;
            }
            else if (packet_str[i] == ']') {
                list_stack.pop();
                i++;
            }
        }

        return *this;
    }

    std::string toString() const {
        std::ostringstream str;
        writeArrVal(values, str);
        return str.str();
    }

    enum CompareResult {
        Equal, InOrder, OutOfOrder
    };

    static CompareResult compare(Packet const & left, Packet const & right) {
        return compare_arr(left.values, right.values);
    }

    friend std::ostream& operator <<(std::ostream& os, Packet const & p) {
        os << p.toString();
        return os;
    }

    friend bool operator < (Packet & a, Packet & b) {
        return Packet::compare(a, b) == Packet::CompareResult::InOrder;
    }

private:
    static void writeArrVal(std::vector<std::any> const & values, std::ostringstream & str) {
        str << "[";

        for (size_t i = 0; i < values.size(); i++) {
            auto const & value = values[i];

            if (value.type() == typeid(int)) {
                int int_val = std::any_cast<int>(value);
                str << int_val;
            }
            else {
                auto const & arr_val = std::any_cast<std::vector<std::any> const &>(value);
                writeArrVal(arr_val, str);
            }

            if (i < values.size() - 1)
                str << ',';
        }

        str << ']';
    }

    static CompareResult compare_arr(std::vector<std::any> const & left, std::vector<std::any> const & right) {
        for (size_t i = 0; i < left.size(); i++) {
            if (i >= right.size())
                return OutOfOrder;

            bool left_is_int = left[i].type() == typeid(int);
            bool right_is_int = right[i].type() == typeid(int);

            if (left_is_int && right_is_int) {
                int left_val = std::any_cast<int>(left[i]);
                int right_val = std::any_cast<int>(right[i]);

                if (left_val < right_val)
                    return InOrder;

                if (left_val > right_val)
                    return OutOfOrder;
            }
            else if (!left_is_int && !right_is_int) {
                auto const & left_arr = std::any_cast<std::vector<std::any> const &>(left[i]);
                auto const & right_arr = std::any_cast<std::vector<std::any> const &>(right[i]);

                auto result = compare_arr(left_arr, right_arr);
                if (result != CompareResult::Equal)
                    return result;
            }
            else if (!left_is_int && right_is_int) {
                auto const & left_arr = std::any_cast<std::vector<std::any> const &>(left[i]);
                std::vector<std::any> right_arr = {std::any_cast<int>(right[i])};

                auto result = compare_arr(left_arr, right_arr);
                if (result != CompareResult::Equal)
                    return result;
            }
            else if (left_is_int && !right_is_int) {
                std::vector<std::any> left_arr = {std::any_cast<int>(left[i])};
                auto const & right_arr = std::any_cast<std::vector<std::any> const &>(right[i]);

                auto result = compare_arr(left_arr, right_arr);
                if (result != CompareResult::Equal)
                    return result;
            }
        }

        if (left.size() < right.size()) {
            return CompareResult::InOrder;
        }

        return CompareResult::Equal;
    }
};
