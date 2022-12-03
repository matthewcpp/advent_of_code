
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>

enum class MoveType {
    Rock = 1, Paper = 2, Scissors = 3
};

enum class Result {
    OpponentWon = 0, Draw = 3, PlayerWon = 6
};

#define GAME_INPUT(player, opponent) ((static_cast<uint32_t>((player)) << 16) | static_cast<uint32_t>((opponent)))

std::unordered_map<uint32_t, Result> game_results = {
    {GAME_INPUT(MoveType::Rock, MoveType::Rock), Result::Draw}, {GAME_INPUT(MoveType::Rock, MoveType::Paper), Result::PlayerWon}, {GAME_INPUT(MoveType::Rock, MoveType::Scissors), Result::OpponentWon},
    {GAME_INPUT(MoveType::Paper, MoveType::Rock), Result::OpponentWon}, {GAME_INPUT(MoveType::Paper, MoveType::Paper), Result::Draw}, {GAME_INPUT(MoveType::Paper, MoveType::Scissors), Result::PlayerWon},
    {GAME_INPUT(MoveType::Scissors, MoveType::Rock), Result::PlayerWon}, {GAME_INPUT(MoveType::Scissors, MoveType::Paper), Result::OpponentWon}, {GAME_INPUT(MoveType::Scissors, MoveType::Scissors), Result::Draw},
};


Result to_result(char val);
MoveType to_opponent_move(char val);
MoveType to_player_move(char val);

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "2.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "2.txt";

    std::ifstream input_file(input_file_path);
    uint32_t score = 0;

    if (!input_file)
        return 1;

    std::string current_line;
    while (std::getline(input_file, current_line)) {
        if (current_line.empty()) {
            continue;
        }

        auto opponent_move = to_opponent_move(current_line[0]);
        auto player_move = to_player_move(current_line[2]);
        auto result = game_results[GAME_INPUT(opponent_move, player_move)];

        score += static_cast<uint32_t>(player_move) + static_cast<uint32_t>(result);
    }

    std::cout << score << std::endl;

    return 0;
}

MoveType to_opponent_move(char val) {
    switch (val) {
        case 'A':
            return MoveType::Rock;
        case 'B':
            return MoveType::Paper;
        case 'C':
            return MoveType::Scissors;
    }
}

MoveType to_player_move(char val) {
    switch (val) {
        case 'X':
            return MoveType::Rock;
        case 'Y':
            return MoveType::Paper;
        case 'Z':
            return MoveType::Scissors;
    }
}