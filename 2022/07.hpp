#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Parser {
    Parser(std::string const & input_file_path) : input_file(input_file_path) {}

    std::ifstream input_file;
    std::string current_line;
    std::filesystem::path cwd;

    struct Directory {
        std::vector<Directory*> child_dirs;
        size_t files_size = 0;

        size_t totalSize() const {
            size_t total_size = files_size;

            for (auto const & child : child_dirs)
                total_size += child->totalSize();

            return total_size;
        }
    };

    std::unordered_map<std::string, std::unique_ptr<Directory>> dir_map;

    void parseInput() {
        std::getline(input_file, current_line);
        while (input_file) {
            processCommand();
        }
    }

    void processCommand() {
        std::istringstream str_stream(current_line);
        char token;
        std::string command;
        str_stream >> token >> command;

        if (command == "cd") {
            std::string dir;
            str_stream >> dir;

            if (dir == "..") {
                cwd = cwd.parent_path();
            }
            else {
                cwd /= dir;

                if (dir_map.empty()) {
                    dir_map[cwd.string()] = std::make_unique<Directory>();
                }
            }

            std::getline(input_file, current_line);
        }
        else if (command == "ls") {
            auto& current_directory = dir_map[cwd.string()];

            while (std::getline(input_file, current_line)) {
                std::istringstream ls_stream(current_line);
                std::string first, second;
                ls_stream >> first >> second;

                if (first == "$") {
                    break;
                }
                else if (first == "dir") {
                    auto dir_path = cwd / second;
                    auto* directory = new Directory();
                    current_directory->child_dirs.push_back(directory);
                    dir_map.insert(std::make_pair(dir_path.string(), directory));
                }
                else {
                    current_directory->files_size += std::stoull(first);
                }
            }
        }
    }

    Directory const * rootDir() const {
        auto result = dir_map.find("/");

        if (result == dir_map.end())
            return nullptr;
        
        return result->second.get();
    }
};
