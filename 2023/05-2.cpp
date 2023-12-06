#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "05.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "05.txt";

struct MapRange {
    MapRange(uint64_t source_start, uint64_t length, uint64_t dest_start) : source_start(source_start), length(length), dest_start(dest_start){}
    uint64_t source_start, length, dest_start;

    inline uint64_t end() const { return source_start + length - 1; }
    inline bool contains(uint64_t val) const {return val >= source_start && val <= end(); }
};

struct SeedRange {
    SeedRange (uint64_t start, uint64_t length) : start(start), length(length) {}
    uint64_t start;
    uint64_t length;

    inline uint64_t end() const { return start + length - 1; }
};

std::pair<std::vector<SeedRange>, std::vector<SeedRange>> remap(const SeedRange& seed_range, const MapRange& range_mapping) {
    std::vector<SeedRange> mapped, unmapped;
    // ignore
    if (seed_range.start > range_mapping.end() || seed_range.end() < range_mapping.source_start) {
        unmapped.push_back(seed_range);
    }

    // totally contains
    else if (seed_range.start < range_mapping.source_start && seed_range.end() > range_mapping.end()) {
        unmapped.emplace_back(seed_range.start, range_mapping.source_start - seed_range.start);
        mapped.emplace_back(range_mapping.dest_start, range_mapping.length);
        unmapped.emplace_back(range_mapping.end() + 1, seed_range.end() - range_mapping.end());
    }

    // seed contained in range
    else if (range_mapping.contains(seed_range.start) && range_mapping.contains(seed_range.end())) {
        mapped.emplace_back(range_mapping.dest_start + (seed_range.start - range_mapping.source_start), seed_range.length);
    }

    // seed starts in range and extends beyond
    else if (range_mapping.contains(seed_range.start) && seed_range.end() > range_mapping.end()) {
        unmapped.emplace_back(range_mapping.end() + 1, seed_range.end() - range_mapping.end());
        mapped.emplace_back(range_mapping.dest_start + seed_range.start - range_mapping.source_start, range_mapping.end() + 1 - seed_range.start);
    }

    // seed starts out of range and ends within
    else if (seed_range.start < range_mapping.source_start && range_mapping.contains(seed_range.end())) {
        unmapped.emplace_back(seed_range.start, range_mapping.source_start - seed_range.start);
        mapped.emplace_back(range_mapping.dest_start, seed_range.length - (range_mapping.source_start - seed_range.start));
    }

    return std::make_pair(mapped, unmapped);
}
int main(int argc, char** argv) {
    std::ifstream file(input_file_path);
    std::string line_str;
    uint64_t dest_start, source_start, length;

    std::vector<SeedRange> seed_ranges;
    std::vector<std::vector<MapRange>> range_maps;

    {
        std::getline(file, line_str);
        std::istringstream line(std::move(line_str));
        line.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        while (line >> source_start >> length) {
            seed_ranges.emplace_back(source_start, length);
        }
    }

    while (std::getline(file, line_str)) {
        if (line_str.empty()) {
            range_maps.emplace_back();
            std::getline(file, line_str);
        }
        else {
            std::istringstream line(std::move(line_str));
            line >> dest_start >> source_start >> length;
            range_maps.back().emplace_back(source_start, length, dest_start);
        }
    }

    for (const auto ranges : range_maps) {
        std::vector<SeedRange> mapped_ranges;
        for (auto const& range_mapping : ranges) {
            std::vector<SeedRange> unmaped_ranges;
            // cut all seeds with a range mapping then update current seed state
            for (auto const& seed_range : seed_ranges) {
                auto [remapped, unmaped] = remap(seed_range, range_mapping);
                mapped_ranges.insert(mapped_ranges.end(), remapped.begin(), remapped.end());
                unmaped_ranges.insert(unmaped_ranges.end(), unmaped.begin(), unmaped.end());
            }

            seed_ranges = unmaped_ranges;
        }

        seed_ranges.insert(seed_ranges.end(), mapped_ranges.begin(), mapped_ranges.end());
    }

    uint64_t min_loc = std::numeric_limits<uint64_t>::max();
    for (const auto& seed_range : seed_ranges) {
        min_loc = std::min(min_loc, seed_range.start);
    }

    std::cout << min_loc << std::endl;

    return 0;
}