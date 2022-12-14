#pragma once

#include <algorithm>
#include <cstdint>
#include <deque>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

struct Point2{
    int x, y;

    Point2(){}
    Point2(int x_val, int y_val) : x(x_val), y(y_val) {}

    Point2& operator +=(Point2 const & pt) {
        x += pt.x;
        y += pt.y;
        return *this;
    }

    friend Point2 operator+(Point2 const & a, Point2 const & b) {
        return Point2(a.x + b.x, a.y + b.y);
    }

    friend Point2 operator-(Point2 const & a, Point2 const & b) {
        return Point2(a.x - b.x, a.y - b.y);
    }

    friend bool operator==(Point2 const & a, Point2 const & b) {
        return a.x == b.x && a.y == b.y;
    }

    friend bool operator!=(Point2 const & a, Point2 const & b) {
        return !(a == b);
    }
};

struct Map {
    Point2 world_min, world_max, world_start_pos;
    std::vector<std::deque<char>> grid;

    inline Point2 toLocalPoint(Point2 const & world_point) const {
        return Point2(world_point.x - world_min.x, world_point.y - world_min.y);
    }

    inline bool isInBounds(Point2 const & pt) const {
        return pt.x >= 0 && pt.x < grid[0].size() && pt.y >= 0 && pt.y < grid.size();
    }

    void parse(std::string const & input_file_path, const Point2& start) {
        std::ifstream input_file(input_file_path);
        std::string current_line, arrow;
        std::vector<std::vector<Point2>> segments;
        int x,  y;
        char comma;

        world_start_pos = start;
        world_min = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
        world_max = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

        updateWorldDimensions(world_start_pos.x, world_start_pos.y);

        while (std::getline(input_file, current_line)) {
            std::istringstream str(current_line);
            auto & segment = segments.emplace_back();

            for (;;) {
                str >> x >> comma >> y;
                segment.emplace_back(x,y);
                updateWorldDimensions(x, y);
                str >> arrow;

                if (!str)
                    break;
            }
        }

        buildGrid(segments);
    }

    std::string toString() const {
        std::ostringstream str;

        for (auto const & row : grid) {
            for (auto const cell : row) {
                str << cell;
            }
            str << std::endl;
        }

        return str.str();
    }

    friend std::ostream& operator<<(std::ostream & os, Map const & m) {
        os << m.toString();
        return os;
    }

private:
    void updateWorldDimensions(int x, int y) {
        world_min.x = std::min(world_min.x, x);
        world_min.y = std::min(world_min.y, y);
        world_max.x = std::max(world_max.x, x);
        world_max.y = std::max(world_max.y, y);
    }

    void buildGrid(std::vector<std::vector<Point2>> const & segments) {
        grid.clear();

        auto size = world_max - world_min;
        size += Point2(1,1);

        for (int y = 0; y < size.y; y++) {
            grid.emplace_back(size.x, '.');
        }

        for (auto const & segment : segments) {
            auto a = toLocalPoint(segment[0]);

            for (size_t i = 1; i < segment.size(); i++) {
                auto b = toLocalPoint(segment[i]);
                insertSegment(a, b);
                a = b;
            }
        }

        auto local_start = toLocalPoint(world_start_pos);
        grid[local_start.y][local_start.x] = '+';
    }

    void insertSegment(Point2 const & a, Point2 const & b) {
        auto dir = b - a;
        dir.x = std::clamp(dir.x, -1, 1);
        dir.y = std::clamp(dir.y, -1, 1);

        Point2 current = a;

        grid[current.y][current.x] = '#';
        do {
            current += dir;
            grid[current.y][current.x] = '#';
        } while (current != b);
    }
};

class Simulate {
public:
    static Point2 sandUnitPosition(Map const & map, Point2 sand_pos, std::vector<Point2>& sand_path) {
        sand_path.clear();

        for (;;) {
            if (!map.isInBounds(sand_pos))
                return sand_pos;

            sand_path.push_back(sand_pos);

            if (check_point(map, sand_pos, 0, 1))
                continue;

            if (check_point(map, sand_pos, -1, 1))
                continue;

            if (check_point(map, sand_pos, 1, 1))
                continue;

            break;
        }

        return sand_pos;
    }

private:
    static bool check_point(Map const& map, Point2 & current_point, int dx, int dy) {
        Point2 target_point = {current_point.x + dx, current_point.y + dy};

        if (!map.isInBounds(target_point) || map.grid[target_point.y][target_point.x] == '.') {
            current_point = target_point;
            return true;
        }

        return false;
    }
};
