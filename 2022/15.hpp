#pragma once

#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

struct Point2 {
    int64_t x, y;

    Point2(){}
    Point2(int64_t x_val, int64_t y_val) : x(x_val), y(y_val) {}

    static uint64_t manhattanDistance(Point2 const & a, Point2 const & b) {
        return static_cast<uint64_t>(std::abs(a.x - b.x)) + static_cast<uint64_t>(std::abs(a.y - b.y));
    }

    static Point2 midpoint(Point2 const & a, Point2 const & b) {
        return Point2((a.x + b.x) / 2, (a.y + b.y) / 2);
    }

    size_t hash() const {
        size_t value = 0;
        std::hash<int64_t> hasher;
        value ^= hasher(x) + 0x9e3779b9 + (value<<6) + (value>>2);
        value ^= hasher(y) + 0x9e3779b9 + (value<<6) + (value>>2);

        return value;
    }

    friend bool operator==(Point2 const & a, Point2 const & b) {
        return a.x == b.x && a.y == b.y;
    }

    friend Point2 operator+(Point2 const & a, Point2 const & b) {
        return Point2(a.x + b.x, a.y + b.y);
    }
};

struct PointHash{
    size_t operator()(Point2 const & pt) const {
        return pt.hash();
    }
};

struct Sensor {
    Sensor(Point2 const & p, uint64_t r) : position(p), radius(r) {}

    Point2 position;
    uint64_t radius;
};

struct Map {
    Point2 min_pos = {std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max()};
    Point2 max_pos = {std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min()};
    std::vector<Sensor> sensors;
    std::unordered_set<Point2, PointHash> beacons;

    void parse(std::string const & input_file_path){

        std::ifstream input_file(input_file_path);
        std::string current_line;

        std::regex input_regex("Sensor at x=(-*\\d+), y=(-*\\d+): closest beacon is at x=(-*\\d+), y=(-*\\d+)");
        std::smatch results;

        while (std::getline(input_file, current_line)) {
            std::regex_match(current_line, results, input_regex);
            Point2 sensor_pos = {std::stoll(results[1]), std::stoll(results[2])};
            Point2 beacon_pos = {std::stoll(results[3]), std::stoll(results[4])};
            auto sensor_radius = Point2::manhattanDistance(sensor_pos, beacon_pos);

            min_pos.x = std::min(min_pos.x, sensor_pos.x - static_cast<int64_t>(sensor_radius));
            min_pos.y = std::min(min_pos.y, sensor_pos.y - static_cast<int64_t>(sensor_radius));
            max_pos.x = std::max(max_pos.x, sensor_pos.x + static_cast<int64_t>(sensor_radius));
            max_pos.y = std::max(max_pos.y, sensor_pos.y + static_cast<int64_t>(sensor_radius));

            sensors.emplace_back(sensor_pos, sensor_radius);
            beacons.insert(beacon_pos);
        }
    }
};
