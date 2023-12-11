/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** ai
*/

#ifndef AI_HPP
#define AI_HPP

#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <functional>

#include "./uint256/uint256_t.h"
#include "./xxhashlib/xxhash.h"
#include "WinChecker.hpp"
#include "ML.hpp"

// #define MAX_DEPTH 4;

class AI {
    public:
        AI();
        ~AI();
        std::pair<int, int> bestMove(std::vector<std::vector<char>> &board, uint256_t &humanBits, uint256_t &cpuBits);
        std::vector<std::pair<int, int>> getSquaresToCheck(const std::vector<std::vector<char>> &my_board);
        void addAdjacent(const char i, const char j, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board);
        void put(const char y, const char x, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board);

        int alphabeta(std::vector<std::vector<char>> matrix, int depth, int alpha, int beta, bool isAiTurn, uint256_t playerBits, uint256_t opponentBits);
        size_t staticEval(const uint256_t &matrix);
        size_t matchMask(uint256_t &mask, const uint256_t &matrix);
        void initialize(void);
        uint countSetBits(unsigned char n);
        size_t staticREval(const uint256_t &matrix, const uint256_t &opponentMatrix);

        XXH64_hash_t hashCalculator(std::vector<std::vector<char>> &board);
        int turnCalculator(std::vector<std::vector<char>> &board);
    protected:
    private:
        ML scores;
        XXH64_state_t *hash_stream;
        // size_t totalCalcs = 0;


        int MAX_DEPTH = 4;
//        int MAX_DEPTH = 2;
        int MY_INFINITY = 100000;
        int MY_NINFINITY = -100000;
};

#endif /* !AI_HPP */
