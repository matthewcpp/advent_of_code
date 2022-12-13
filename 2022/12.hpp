#pragma once

#include <cstdint>
#include <cmath>
#include <fstream>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

struct Point2 {
    int x, y;

    Point2(){}
    Point2(int x_val, int y_val) : x(x_val), y(y_val){}

    static uint32_t manhattanDistance(Point2 const & a, Point2 const & b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    friend bool operator==(Point2 const & a, Point2 const & b) {
        return a.x == b.x && a.y == b.y;
    }

    uint64_t hash() const {
        uint64_t value;
        int* arr = reinterpret_cast<int*>(&value);
        std::memcpy(arr, this, sizeof(Point2));
        return value;
    }
};

struct PointHash{
    size_t operator()(Point2 const & pt) const {
        return static_cast<size_t>(pt.hash());
    }
};

struct Map {
    std::vector<std::string> grid;
    Point2 start, exit;

    void extract_map_point(Point2 & pt, char search, char replace) {
        auto & row = grid.back();
        auto index = row.find_first_of(search);

        if (index == std::string::npos) {
            return;
        }

        pt.x = static_cast<int>(index);
        pt.y = static_cast<int>(grid.size() - 1);

        row[index] = replace;
    }

    void load(std::string const & input_file_path) {
        std::ifstream input_file(input_file_path);
        std::string current_line;

        while (std::getline(input_file, current_line)) {
            grid.push_back(current_line);
            extract_map_point(start, 'S', 'a');
            extract_map_point(exit, 'E', 'z');
        }
    }

    bool is_in_bounds(Point2 const & pos) const{
        return pos.x >= 0 && pos.x < grid[0].size() && pos.y >= 0 && pos.y < grid.size();
    }
};

struct Solver {
    void update_f_score(Point2 const & pos, uint32_t f_score) {
        auto existing_f_result = f_scores.find(pos);

        if (existing_f_result != f_scores.end()) {
            auto & f_arr = open_set[existing_f_result->second];
            f_arr.erase(std::find(f_arr.begin(), f_arr.end(), pos));

            if (f_arr.empty())
                open_set.erase(existing_f_result->second);
        }

        f_scores[pos] = f_score;
        open_set[f_score].push_back(pos);
    }

    void try_move(Point2& current, int delta_x, int delta_y) {
        Point2 target_pos {current.x + delta_x, current.y + delta_y};

        if (!map.is_in_bounds(target_pos))
            return;

        auto current_space = map.grid[current.y][current.x];
        auto target_space = map.grid[target_pos.y][target_pos.x];

        if (target_space - current_space > 1 )
            return;

        auto current_g_score = g_scores[current];
        auto target_g_score = current_g_score + 1U;
        auto current_result = g_scores.find(target_pos);

        if (current_result == g_scores.end() || current_result->second > target_g_score) {
            auto target_f_score = target_g_score + Point2::manhattanDistance(target_pos, map.exit);
            update_f_score(target_pos, target_f_score);
            g_scores[target_pos] = target_g_score;
            previous_nodes[target_pos] = current;
        }
    }

    Solver(Map const & m) : map(m) {}

    Point2 getNext() {
        auto& lowest_arr = open_set.begin()->second;

        Point2 pt = lowest_arr.back();
        lowest_arr.pop_back();

        if (lowest_arr.empty())
            open_set.erase(open_set.begin());

        f_scores.erase(pt);
        
        return pt;
    }

    bool solve(const Point2& start_pos, const Point2& exit_pos) {
        update_f_score(start_pos, 0);
        g_scores[start_pos] = 0;
        
        do {
            Point2 current = getNext();

            if (current == exit_pos) {
                break;
            }

            try_move(current, 1, 0);
            try_move(current, -1, 0);
            try_move(current, 0, 1);
            try_move(current, 0, -1);
        } while (!open_set.empty());

        return previous_nodes.find(map.exit) != previous_nodes.end();
    }

    std::vector<Point2> getPath() {
        std::vector<Point2> path;

        if (!previous_nodes.count(map.exit))
            return path;

        auto previous_node_result = previous_nodes.find(map.exit);
        path.push_back(map.exit);

        while (previous_node_result != previous_nodes.end()) {
            path.push_back(previous_node_result->second);
            previous_node_result = previous_nodes.find(previous_node_result->second);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    Map const & map;
    std::unordered_map<Point2, uint32_t, PointHash> g_scores;
    std::unordered_map<Point2, uint32_t, PointHash> f_scores;
    std::unordered_map<Point2, Point2, PointHash> previous_nodes;
    std::map<uint32_t, std::vector<Point2>> open_set;
};