#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "10-2.test1.txt";
//auto const input_file_path = std::string(INPUT_DIRECTORY) + "10-2.test2.txt";
//auto const input_file_path = std::string(INPUT_DIRECTORY) + "10-2.test3.txt";
//auto const input_file_path = std::string(INPUT_DIRECTORY) + "10-2.test4.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "10.txt";

struct Vec2{ 
    Vec2() = default;
    Vec2(int x, int y) : x(x), y(y) {}
    inline bool isZero() const { return x == 0 && y == 0; }
    int x, y;
};

bool operator< (const Vec2& a, const Vec2& b) { return a.y != b.y ? a.y < b.y : a.x < b.x; }
Vec2 operator- (const Vec2& a, const Vec2& b) { return Vec2(a.x - b.x, a.y - b.y); }
Vec2 operator+ (const Vec2& a, const Vec2& b) { return Vec2(a.x + b.x, a.y + b.y); }
bool operator== (const Vec2& a, const Vec2& b) { return a.x == b.x && a.y == b.y; }

struct World {
    std::vector<std::string> grid;
    Vec2 start;
};
struct Path {
    Path() = default;
    Path(const Vec2& current, const Vec2& direction) : direction(direction), initial_direction(direction), steps(1, current) {}
    Vec2 direction, initial_direction;
    std::vector<Vec2> steps;
};

bool pos_is_valid(const std::vector<std::string>& grid, const Vec2 target) {
    if (target.y < 0 || target.y >= grid.size() || 
        target.x < 0 || target.x >= grid[target.y].length()) {
        return false;
    }

    return true;
}

Vec2 follow_path(const World& world, const Vec2& current, const Vec2& direction, const Vec2 target) {
    Vec2 new_direction(0, 0);

    if (!pos_is_valid(world.grid, target)) {
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

static const std::array<Vec2, 4> directions = {{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} }};

std::vector<Path> find_loop(const World& world) {
    std::vector<Path> paths, active_paths;
    
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
                active_path.initial_direction = path.initial_direction;
                active_path.steps = std::move(path.steps);
                active_path.steps.emplace_back(target);
            }
        }

        std::swap(paths, active_paths);

        if (paths.empty() || loop_found(paths)) {
            break;
        }
    }

    return paths;
}

char determine_start_char(const Path& a, const Path& b) {
    const auto check = [] (const Vec2& a, const Vec2& b, int x, int y){
        return (a.x == x && b.y == y) || (b.x == x && a.y == y);
    };

    if (a.initial_direction.x == 0 && b.initial_direction.x == 0) {
        return '|';
    } else if (a.initial_direction.y == 0 && b.initial_direction.y == 0) {
        return '-';
    } else if (check(a.initial_direction, b.initial_direction, 1, 1)) {
        return 'F';
    } else if (check(a.initial_direction, b.initial_direction, 1, -1)) {
        return '7';
    } else if (check(a.initial_direction, b.initial_direction, -1, -1)) {
        return 'J';
    } else if (check(a.initial_direction, b.initial_direction, -1, 1)) {
        return 'L';
    } else {
        return '.'; // should not happen
    }
}

const std::string pipe_chars = "|-F7JL";

// everyhting touching this hasnt been processed or is on the loop
bool is_contained(const std::vector<std::string>& grid, const Vec2& pos) {
    static const std::string placeholder_chars = ".I";

    for (const auto& direction : directions) {
        auto target = pos + direction;
        bool contained = false;

        while (pos_is_valid(grid, target)) {
            auto val = grid[target.y][target.x];

            if (pipe_chars.find(val) != std::string::npos) {
                contained = true;
                break;
            }

            target = target + direction;
        }

        if (!contained) {
            return false;
        }
    }

    return true;
}

void fill_pos(std::vector<std::string>& grid, const Vec2& pos) {
    static const std::string fill_targets = ".I";
    std::vector<Vec2> fill_queue;
    fill_queue.push_back(pos);

    while (!fill_queue.empty()) {
        auto pos = fill_queue.back();
        fill_queue.pop_back();

        grid[pos.y][pos.x] = 'O';

        for (const auto& direction : directions) {
            auto target = pos + direction;

            if (pos_is_valid(grid, target) && fill_targets.find(grid[target.y][target.x]) != std::string::npos) {
                grid[target.y][target.x] = 'Q';
                fill_queue.push_back(target);
            }
        }
    }
}

std::vector<std::string> create_flood_grid(const World& world, const std::vector<Path>& loop_paths) {
    std::vector<std::string> flooded_gird;
    for (size_t i = 0; i < world.grid.size() * 2; i++) {
        flooded_gird.emplace_back(world.grid[0].size() * 2, '.');
    }
    // mark path
    for (const auto& path : loop_paths) {
        for (const auto& step : path.steps) {
            int flooded_y = step.y * 2, flooded_x = step.x * 2;

            switch (world.grid[step.y][step.x]) {
                case '-':
                    flooded_gird[flooded_y][flooded_x] = '-';
                    flooded_gird[flooded_y][flooded_x + 1] = '-';
                    break;

                case '|':
                    flooded_gird[flooded_y][flooded_x] = '|';
                    flooded_gird[flooded_y + 1][flooded_x] = '|';
                    break;

                case 'F':
                    flooded_gird[flooded_y][flooded_x] = 'F';
                    flooded_gird[flooded_y][flooded_x + 1] = '-';
                    flooded_gird[flooded_y + 1][flooded_x] = '|';
                    break;

                case '7':
                    flooded_gird[flooded_y][flooded_x] = '7';
                    flooded_gird[flooded_y + 1][flooded_x] = '|';
                    break;

                case 'J':
                    flooded_gird[flooded_y][flooded_x] = 'J';
                    break;

                case 'L':
                    flooded_gird[flooded_y][flooded_x] = 'L';
                    flooded_gird[flooded_y][flooded_x + 1] = '-';
                    break;
            }
        }
    }

    return flooded_gird;
}

void flood_grid(std::vector<std::string>& flooded_gird) {
    for (int y = 0; y < flooded_gird.size(); y++) {
        for (int x = 0; x < flooded_gird[0].size(); x++) {
            if (flooded_gird[y][x] != '.') {
                continue;
            }

            Vec2 pos(x, y);
            if (is_contained(flooded_gird, pos)) {
                flooded_gird[y][x] = 'I';
            } else {
                fill_pos(flooded_gird, pos);
            }
        }
    }
}

int main (int argc, char** argv) {
    World world = parse_world_map(input_file_path);
    const auto loop_paths = find_loop(world);
    world.grid[world.start.y][world.start.x] = determine_start_char(loop_paths[0], loop_paths[1]);
    auto flooded_gird = create_flood_grid(world, loop_paths);
    flood_grid(flooded_gird);

    size_t contained_count = 0;

    for (size_t y = 0; y < flooded_gird.size(); y += 2) {
        for (size_t x = 0; x < flooded_gird[y].size(); x += 2) {
            if (flooded_gird[y][x] == 'I') {
                contained_count += 1;
            }
        }
    }

    std::cout << contained_count << std::endl;

    return 0;
}