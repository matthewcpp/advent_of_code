#pragma once

#include <stdint.h>

template <typename T>
struct IntVec2{
    using Type = typename T;

    IntVec2() = default;
    IntVec2(T x, T y) : x(x), y(y) {}
    inline bool isZero() const { return x == 0 && y == 0; }
    T x, y;

    bool operator< (const IntVec2<T>& other) const { 
        return y != other.y ? y < other.y : x < other.x; 
    }
    
    IntVec2<T> operator- (const IntVec2& other) const { 
        return IntVec<T>(x - other.x, y - other.y); 
    }

    IntVec2<T> operator+ (const IntVec2& other) const { 
        return IntVec<T>(x + other.x, y + other.y); 
    }
    bool operator== (const IntVec2& other) const { 
        return x == other.x && y == other.y;
    }

    static T ManhattanDistance(const IntVec2<T>& a, const IntVec2<T>& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }
};

typedef IntVec2<int64_t> IVec2;
typedef IntVec2<uint64_t> UVec2;

typedef IntVec2<int32_t> I32Vec2;
typedef IntVec2<uint32_t> U32Vec2;
