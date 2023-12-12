#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "10.test1.txt";
//auto const input_file_path = std::string(INPUT_DIRECTORY) + "10.test2.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "10.txt";

struct Vec2{ 
    Vec2() = default;
    Vec2(int x, int y) : x(x), y(y) {}
    inline bool isZero() const { return x == 0 && y == 0; }
    int x, y; 
};

Vec2 operator- (const Vec2& a, const Vec2& b) { return Vec2(a.x - b.x, a.y - b.y); }
Vec2 operator+ (const Vec2& a, const Vec2& b) { return Vec2(a.x + b.x, a.y + b.y); }
bool operator== (const Vec2& a, const Vec2& b) { return a.x == b.x && a.y == b.y; }

struct World {
    std::vector<std::string> grid;
    Vec2 start;
};
struct Path {
    Path() = default;
    Path(const Vec2& current, const Vec2& direction) : direction(direction), steps(1, current) {}
    Vec2 direction;
    std::vector<Vec2> steps;
};

bool pos_is_valid(const World& world, const Vec2 target) {
    if (target.y < 0 || target.y >= world.grid.size() || 
        target.x < 0 || target.x >= world.grid[target.y].length()) {
        return false;
    }

    return true;
}

Vec2 follow_path(const World& world, const Vec2& current, const Vec2& direction, const Vec2 target) {
    Vec2 new_direction(0, 0);

    if (!pos_is_valid(world, target)) {
        return new_direction;
    }

    const auto target_type = world.grid[target.y][target.x];

    switch (target_type) {
        case 'S':
            return direction;

        case '|':
            if (direction.x == 0) {
                new_direction.y = direction.y;
            }
            break;

        case '-':
            if (direction.y == 0) {
                new_direction.x = direction.x;
            }
            break;

        case 'L':
            if (direction.y == 1 && direction.x == 0) {
                new_direction.x = 1;
            } else if (direction.y == 0 && direction.x == -1) {
                new_direction.y = -1;
            }
            break;
        
        case 'J':
            if (direction.y == 1 && direction.x == 0) {
                new_direction.x = -1;
            } else if (direction.y == 0 && direction.x == 1) {
                new_direction.y = -1;
            }
            break;

        case '7':
            if (direction.y == -1 && direction.x == 0) {
                new_direction.x = -1;
            } else if (direction.y == 0 && direction.x == 1) {
                new_direction.y = 1;
            }
            break;

        case 'F':
            if (direction.y == -1 && direction.x == 0) {
                new_direction.x = 1;
            } else if (direction.y == 0 && direction.x == -1) {
                new_direction.y = 1;
            }
            break;
    }

    // '.'
    return new_direction;
}

bool loop_found(std::vector<Path>& active_paths) {
    for (int i = 0; i < active_paths.size(); i++) {
        for (int j = 0; j < active_paths.size(); j++) {
            if (i != j && active_paths[i].steps.back() == active_paths[j].steps.back()) {
                return true;
            }
        }
    }

    return false;
}

World parse_world_map(const std::string& input_file_path) {
    World world;
    std::ifstream file(input_file_path);
    std::string line;
    bool start_found = false;
    while (std::getline(file, line)) {
        world.grid.emplace_back(std::move(line));

        if (start_found) {
            continue;
        }

        auto start_index = world.grid.back().find('S');
        if (start_index != std::string::npos) {
            world.start.x = static_cast<int>(start_index);
            world.start.y = static_cast<int>(world.grid.size() - 1);
            start_found = true;
        }
    }

    return world;
}

std::vector<Path> find_loop(const World& world) {
    std::vector<Path> paths, active_paths;
    
    static const std::array<Vec2, 4> directions = {{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} }};
    for (const auto& direction : directions) {
        paths.emplace_back(world.start, direction);
    }

    for (;;) {
        active_paths.clear();

        for (auto& path : paths) {
            auto target = path.steps.back() + path.direction;
            auto new_direction = follow_path(world, path.steps.back(), path.direction, target);

            if (!new_direction.isZero()) {
                auto& active_path = active_paths.emplace_back();
                active_path.direction = new_direction;
                active_path.steps = std::move(path.steps);
                active_path.steps.emplace_back(target);
            }
        }

        std::swap(paths, active_paths);

        if (loop_found(paths)) {
            break;
        }
    }

    return paths;
}

int main (int argc, char** argv) {
    World world = parse_world_map(input_file_path);
    const auto loop_paths = find_loop(world);

    std::cout << loop_paths[0].steps.size() - 1 << std::endl;


    return 0;
}