/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include "WinChecker.hpp"
#include "ai.hpp"

class Game {
    public:
        Game();
        ~Game();

        int LaunchPlayerGame();
        int LaunchAIGame(AI &player_1);
        void LaunchXAIGame(size_t X);

    protected:
    private:
        void display_board(std::vector<std::vector<char>> &board);
        bool put_piece(std::vector<std::vector<char>> &board, int &x, int &y, char piece);
        uint256_t bits_calculator(std::vector<std::vector<char>> &board, char piece);

        bool game_on = true;
        std::vector<std::vector<char>> board{15, std::vector<char>(15, 0)};
        uint256_t Player_1_bits = 0;
        uint256_t Player_2_bits = 0;
        WinChecker check;
        bool is_piece;
        AI player_2;
};

#endif /* !GAME_HPP */
