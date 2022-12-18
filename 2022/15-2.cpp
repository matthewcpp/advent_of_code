#include "15.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

struct Solver {
    Solver(Map const & m) : map(m) {}

    Map const & map;
    std::vector<std::thread> workers;
    size_t next_sensor_index = 0;
    std::mutex next_sensor_index_mutex;
    std::array<Point2, 4> const point_deltas = {{{-1, 1}, {1,1}, {-1, -1}, {1, -1}}};
    int64_t max_search_x, max_search_y;

    void solve(size_t worker_count, int64_t max_x, int64_t max_y) {
        max_search_x = max_x;
        max_search_y = max_y;

        workers.resize(worker_count);

        for (size_t i = 0; i < worker_count; i++) {
            workers[i] = std::thread(&Solver::workerMain, this);
        }

        for (size_t i = 0; i < worker_count; i++) {
            workers[i].join();
        }
    }

private:
    void workerMain() {
        size_t sensor_index;

        for(;;) {
            {
                std::lock_guard<std::mutex> guard(next_sensor_index_mutex);

                if (next_sensor_index == map.sensors.size()) {
                    return;
                }

                sensor_index = next_sensor_index++;
            }

            auto const & sensor = map.sensors[sensor_index];
            int64_t search_radius = sensor.radius + 1;

            for (int64_t y = 0; y <= search_radius; y++) {
                int64_t dx = search_radius - y;

                for (auto const & pos_delta : point_deltas) {
                    Point2 const pt = {sensor.position.x + (dx * pos_delta.x), sensor.position.y + (y * pos_delta.y)};
                    checkPoint(pt);
                }
            }
        }
    }

    void checkPoint(Point2 const & pt) {
        if (pt.x < 0 || pt.x > max_search_x || pt.y < 0 || pt.y > max_search_y) {
            return;
        }

        for (auto const & sensor : map.sensors) {
            if (Point2::manhattanDistance(pt, sensor.position) <= sensor.radius) {
                return;
            }
        }

        {
            std::lock_guard<std::mutex> guard(next_sensor_index_mutex);
            next_sensor_index = map.sensors.size();
            uint64_t tuning_freq = pt.x * 4000000 + pt.y;
            std::cout << pt.x << "," << pt.y << ":\t" << tuning_freq << std::endl;
        }
    }
};

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "15.test.txt";
    //uint64_t max_x = 20ll, max_y = 20ll;

    auto const input_file_path = std::string(INPUT_DIRECTORY) + "15.txt";
    uint64_t max_x = 4000000ll, max_y = 4000000ll;

    Map map;
    map.parse(input_file_path);

    Solver solver(map);
    solver.solve(5, max_x, max_y);

    return 0;
}