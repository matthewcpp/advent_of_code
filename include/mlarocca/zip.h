#pragma once

#include <tuple>
#include <vector>

namespace mlarocca {


template <typename A, typename B>
class zip {
public:
    struct zip_itr {
        zip_itr(typename std::vector<A>::const_iterator a, typename std::vector<B>::const_iterator b) : a_itr(a), b_itr(b) {}

        bool operator!=(const zip_itr& other) {
            return a_itr != other.a_itr || b_itr != other.b_itr;
        }

        zip_itr operator++() {
            ++a_itr;
            ++b_itr;
            return *this;
        }

        auto operator*(){
            return std::tie(*a_itr, *b_itr);
        }

        typename std::vector<A>::const_iterator a_itr;
        typename std::vector<B>::const_iterator b_itr;
    };

public:
    zip(const std::vector<A>& a, const std::vector<B>& b) : a(a), b(b) {}

    zip_itr begin() {
        return zip_itr(a.begin(), b.begin());
    }

    zip_itr end() {
        return zip_itr(a.end(), b.end());
    }

private:
    const std::vector<A>& a;
    const std::vector<B>& b;
};

}
