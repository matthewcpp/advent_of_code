#pragma once

#include <cmath>
#include <cstdint>

struct Vec2 {
    int x, y;

    uint64_t hash() const {
        uint64_t val = 0;
        int* arr = reinterpret_cast<int*>(&val);
        arr[0] = x;
        arr[1] = y;

        return val;
    }

    Vec2& operator += (Vec2 const & b) {
        x += b.x;
        y += b.y;

        return *this;
    }

    bool operator == (Vec2 const & other) const {
        return x == other.x && y == other.y;
    }

    Vec2 operator- (Vec2 const & b) const{
    return Vec2{x - b.x, y - b.y};
}

    void setDirection(char direction) {
        switch(direction) {
            case 'R':
                x = 1; y = 0;
            break;

            case 'L':
                x = -1; y = 0;
            break;

            case 'U':
                x = 0; y = 1;
            break;

            case 'D':
                x = 0; y = -1;
            break;
        }
    }

    static Vec2 abs(Vec2 const & v) {
        return Vec2{std::abs(v.x), std::abs(v.y)};
    }
};
