/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** ai
*/

#ifndef AI_HPP
#define AI_HPP

#include <vector>
#include <list>
#include <array>
#include <tuple>
#include <algorithm>
#include <iostream>

#include "./uint256/uint256_t.h"
// #include "Score.hpp"
#include "WinChecker.hpp"
#include "./include/ThreadPool.hpp"
#include "./include/GameState.hpp"
#include "./include/circular_buffer.hpp"

#define MAX_GAME_MEM 200

class AI {
    public:
        AI(std::vector<std::vector<char>> board, char Ai_color, char Enemy_Color);
        ~AI();
        std::pair<int, int> bestMove(uint256_t humanBits, uint256_t cpuBits);
        std::vector<std::pair<int, int>> getSquaresToCheck(const std::vector<std::vector<char>> &my_board);
        void addAdjacent(const char i, const char j, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board);
        void put(const char y, const char x, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board);

        int alphabeta(std::vector<std::vector<char>> matrix, int depth, int alpha, int beta, bool isAiTurn, uint256_t playerBits, uint256_t opponentBits);
        bool checkWinner(uint256_t &bits, int &depth);
        bool hasWon(const uint256_t &matrix);
        bool matchBitmask(const uint256_t &matrix, uint256_t &mask);
        size_t staticEval(const uint256_t &matrix);
        size_t matchMask(uint256_t &mask, const uint256_t &matrix);
        // size_t calcVertical(uint256_t vMask, const uint256_t &matrix);
        // size_t calcStreak(int streak);

        int alphabeta_thread(std::list<ThreadPool>::iterator thread_it);



        // std::pair<int, int> getNextMove();
        // std::vector<std::pair<int, int>> getSquaresToCheck(std::vector<std::vector<char>> &my_board);
        // bool isTouchingOccupied(const int &x, const int &y) const;
        // bool occupied(const int &x, const int &y) const;

        // int alphabeta(std::vector<std::vector<char>> &board, int depth, int alpha, int beta, bool isAiTurn);
        // int staticEval(std::vector<std::vector<char>> &my_board);
        // int horizontalScore(std::vector<std::vector<char>> &my_board);
        // int verticalScore(std::vector<std::vector<char>> &my_board);
        // int diagonalScore(std::vector<std::vector<char>> &my_board);
        // void scoreConsecutive(char &block, char &current, size_t &streak, int &score);
        // int adjacentBlockScore(const size_t &count) const;

    protected:
    private:
        std::vector<std::vector<char>> _board;
        char _AI_Color;
        char _Enemy_Color;
        // size_t totalCalcs = 0;
        std::list<ThreadPool> thread_pool; // why not a vector or array ???
        // std::list<GameState> thread_memory_space; // HAS TO BE A LIST!!!
        Circular_buffer<GameState *, MAX_GAME_MEM> game_memory_space_waiting;
        Circular_buffer<GameState *, MAX_GAME_MEM> game_memory_space_working;
        // maybe add a pool of init GameState here to save time.

        int MAX_DEPTH = 4;
        int INFINITY = 100000;
        int NINFINITY = -100000;
};

#endif /* !AI_HPP */