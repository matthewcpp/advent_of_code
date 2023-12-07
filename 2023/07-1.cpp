#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

//auto const input_file_path = std::string(INPUT_DIRECTORY) + "07.test.txt";
auto const input_file_path = std::string(INPUT_DIRECTORY) + "07.txt";

enum Kind {HighCard, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind};
const std::unordered_map<char, int> card_ranks = {
    {'A', 13}, {'K', 12}, {'Q', 11}, {'J', 10}, {'T', 9}, 
    {'9', 8}, {'8', 7}, {'7', 6}, {'6', 5}, {'5', 4}, {'4', 3}, {'3', 2}, {'2', 1}
};

struct Hand {
    std::string cards;
    uint64_t bid;
    Kind kind;
};

Kind determine_kind(const std::string& cards) {
    using CardInfo = std::pair<char, int>;
    std::vector<CardInfo> card_counts;

    for (const auto card : cards) {
        auto result = std::find_if(card_counts.begin(), card_counts.end(), [card] (const CardInfo& ci){ return ci.first == card; });
        if (result == card_counts.end()) {
            card_counts.emplace_back(std::make_pair(card, 1));
        } else {
            (*result).second += 1;
        }
    }

    std::sort(card_counts.begin(), card_counts.end(), [](const CardInfo& a, const CardInfo& b){ return a.second > b.second;} );

    if (card_counts.size() == 1) {
        return FiveOfAKind;
    } else if (card_counts.size() == 2 && card_counts[0].second == 4) {
        return FourOfAKind;
    } else if (card_counts.size() == 2 && card_counts[0].second == 3) {
        return FullHouse;
    } else if (card_counts.size() == 3 && card_counts[0].second == 3) {
        return ThreeOfAKind;
    } else if (card_counts.size() == 3 && card_counts[0].second == 2 && card_counts[1].second == 2) {
        return TwoPair;
    } else if (card_counts.size() == 4 && card_counts[0].second == 2) {
        return OnePair;
    } else {
        return HighCard;
    }
}

bool sort_hands_by_strength (const Hand& a, const Hand& b) {
    if (a.kind != b.kind) {
        return a.kind < b.kind;
    }

    for(int i = 0; i < 5; i++) {
        auto rank_a = card_ranks.find(a.cards[i])->second;
        auto rank_b = card_ranks.find(b.cards[i])->second;
        
        if (rank_a == rank_b) {
            continue;
        }

        return rank_a < rank_b;
    }

    return true;
}

int main(int argc, char** argv) {
    std::vector<Hand> hands;
    std::ifstream file(input_file_path);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream str(std::move(line));
        auto & hand = hands.emplace_back();
        str >> hand.cards >> hand.bid;
        hand.kind = determine_kind(hand.cards);
    }

    std::sort(hands.begin(), hands.end(), sort_hands_by_strength);

    size_t winnings = 0, rank = 1;
    for (const auto hand : hands) {
        winnings += hand.bid * rank++;
    }

    std::cout << winnings << std::endl;

    return 0;
}