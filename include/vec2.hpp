#pragma once

#include "hash_combine.hpp"

#include <ostream>
#include <stdint.h>

template <typename T>
struct IntVec2{
    using Type = T;

    IntVec2() = default;
    IntVec2(T x, T y) : x(x), y(y) {}
    inline bool isZero() const { return x == 0 && y == 0; }
    T x, y;

    bool operator< (const IntVec2<T>& other) const { 
        return y != other.y ? y < other.y : x < other.x; 
    }
    
    IntVec2<T> operator- (const IntVec2<T>& other) const { 
        return IntVec2<T>(x - other.x, y - other.y); 
    }

    IntVec2<T> operator-= (const IntVec2<T>& other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    IntVec2<T> operator+ (const IntVec2<T>& other) const { 
        return IntVec2<T>(x + other.x, y + other.y); 
    }

    IntVec2<T> operator+= (const IntVec2<T>& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    bool operator== (const IntVec2<T>& other) const { 
        return x == other.x && y == other.y;
    }

    bool operator!=(const IntVec2<T>& other) const {
        return !(*this == other);
    }

    static T ManhattanDistance(const IntVec2<T>& a, const IntVec2<T>& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }
};

template<typename T>
struct std::hash<IntVec2<T>> {
    size_t operator()(const IntVec2<T>& v) const {
        size_t h = 0;
        hash_combine(h, v.x, v.y);
        return h;
    }
};

template<typename T>
std::ostream& operator << (std::ostream& stream, const IntVec2<T>& v) {
    stream << '[' << v.x << ", " << v.y << ']';

    return stream;
}


typedef IntVec2<int64_t> IVec2;
typedef IntVec2<uint64_t> UVec2;

typedef IntVec2<int32_t> I32Vec2;
typedef IntVec2<uint32_t> U32Vec2;
