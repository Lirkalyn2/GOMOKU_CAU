/*
** EPITECH PROJECT
2023
** GOMOKU_CAU
** File description:
** GameState
*/

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <vector>
#include <shared_mutex>

#include "../uint256/uint256_t.h"

class GameState { // 2856 bits or 357 bytes + 8 for the pointer
    public:
        // what is need to compute.
        std::vector<std::vector<char>> matrix{15, std::vector<char>(15)}; // 15*15*8 = 1800
        int depth; // 32
        int alpha; // 32
        int beta; // 32
        bool isAiTurn;
        uint256_t playerBits; // 256
        uint256_t opponentBits; // 256
        XXH64_hash_t boardHash; // 64
        // std::shared_mutex gameStateProtector; // 448
        std::vector<std::pair<std::pair<int, int>, int>>::iterator move;

        // Compute result;
        // bool isComputeDone; // 0 waitng, 1 done
        // vector to score (iterator/pointer) need to compute our score
        // std::vector<GameState *> results;
        // int score;
};

#endif /* !GAME_STATE_HPP */
