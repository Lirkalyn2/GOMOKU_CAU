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
#include <iostream>
#include "Score.hpp"
#include "WinChecker.hpp"

// #define MAX_DEPTH 4;

class AI {
    public:
        AI(std::vector<std::vector<char>> board, char Ai_color, char Enemy_Color);
        ~AI();
        std::pair<int, int> getNextMove();
        std::vector<std::pair<int, int>> getSquaresToCheck(std::vector<std::vector<char>> &my_board);
        bool isTouchingOccupied(const int &x, const int &y) const;
        bool occupied(const int &x, const int &y) const;

        int alphabeta(std::vector<std::vector<char>> &board, int depth, int alpha, int beta, bool isAiTurn);
        int staticEval(std::vector<std::vector<char>> &my_board);
        int horizontalScore(std::vector<std::vector<char>> &my_board);
        int verticalScore(std::vector<std::vector<char>> &my_board);
        int diagonalScore(std::vector<std::vector<char>> &my_board);
        void scoreConsecutive(char &block, char &current, size_t &streak, int &score);
        int adjacentBlockScore(const size_t &count) const;

    protected:
    private:
        std::vector<std::vector<char>> _board;
        char _AI_Color;
        char _Enemy_Color;


        int MAX_DEPTH = 4;
        int INFINITY = 100000;
        int NINFINITY = -100000;
};

#endif /* !AI_HPP */