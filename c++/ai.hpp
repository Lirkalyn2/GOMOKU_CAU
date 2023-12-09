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
#include <iterator>

#include "./uint256/uint256_t.h"
// #include "Score.hpp"
#include "WinChecker.hpp"
#include "./include/ThreadPool.hpp"
#include "./include/GameState.hpp"
#include "./include/circular_buffer.hpp"

#define MAIN_THREADS_NUMBER 8
#define LEAVES_THREADS_NUMBER 24
#define MAX_MAIN_THREAD_MEM 50
#define MAX_LEAVES_MEM 200

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
        int leaves_thread(std::list<ThreadPool>::iterator thread_it);



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
        std::list<ThreadPool> main_square_thread_pool; // why not a vector or array ??? // they need their own shared mutex.
        std::list<ThreadPool> leaves_thread_pool; // why not a vector or array ???
        // // std::list<GameState> main_square_memory_space; // HAS TO BE A LIST!!!
        Circular_buffer<GameState *, MAX_MAIN_THREAD_MEM> main_square_memory_space;
        Circular_buffer<GameState *, MAX_LEAVES_MEM> leaves_memory_space;
        // maybe add a pool of init GameState here to save time.
        std::shared_mutex main_square_flag;

        int MAX_DEPTH = 4;
        int INFINITY = 100000;
        int NINFINITY = -100000;
};

#endif /* !AI_HPP */