
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

#define PLAYER_MOVE(opponent, desired_result) ((static_cast<uint32_t>((opponent)) << 16) | static_cast<uint32_t>((desired_result)))
std::unordered_map<uint32_t, MoveType> player_move_lookup = {
    {PLAYER_MOVE(MoveType::Rock, Result::Draw), MoveType::Rock}, {PLAYER_MOVE(MoveType::Rock, Result::PlayerWon), MoveType::Paper}, {PLAYER_MOVE(MoveType::Rock, Result::OpponentWon), MoveType::Scissors},
    {PLAYER_MOVE(MoveType::Paper, Result::Draw), MoveType::Paper}, {PLAYER_MOVE(MoveType::Paper, Result::PlayerWon), MoveType::Scissors}, {PLAYER_MOVE(MoveType::Paper, Result::OpponentWon), MoveType::Rock},
    {PLAYER_MOVE(MoveType::Scissors, Result::Draw), MoveType::Scissors}, {PLAYER_MOVE(MoveType::Scissors, Result::PlayerWon), MoveType::Rock}, {PLAYER_MOVE(MoveType::Scissors, Result::OpponentWon), MoveType::Paper},
};


Result to_result(char val);
MoveType to_move(char val);

int main(int argc, char** argv) {
    //auto const input_file_path = std::string(INPUT_DIRECTORY) + "02.test.txt";
    auto const input_file_path = std::string(INPUT_DIRECTORY) + "02.txt";

    std::ifstream input_file(input_file_path);
    uint32_t score = 0;

    if (!input_file)
        return 1;

    std::string current_line;
    while (std::getline(input_file, current_line)) {
        if (current_line.empty()) {
            continue;
        }

        auto opponent_move = to_move(current_line[0]);
        auto desired_result = to_result(current_line[2]);
        auto player_move = player_move_lookup[PLAYER_MOVE(opponent_move, desired_result)];

        score += static_cast<uint32_t>(player_move) + static_cast<uint32_t>(desired_result);
    }

    std::cout << score << std::endl;

    return 0;
}

Result to_result(char val) {
    switch (val) {
        case 'X':
            return Result::OpponentWon;
        case 'Y':
            return Result::Draw;
        case 'Z':
            return Result::PlayerWon;
    }
}

MoveType to_move(char val) {
    switch (val) {
        case 'A':
            return MoveType::Rock;
        case 'B':
            return MoveType::Paper;
        case 'C':
            return MoveType::Scissors;
    }
}