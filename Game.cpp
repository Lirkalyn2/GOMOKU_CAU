/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** Game
*/

#include "Game.hpp"

Game::Game() {}

Game::~Game() {}

int Game::LaunchPlayerGame()
{
    player_2.scores.readFile("./ML_files/random-file");

    while (game_on)
    {
        std::string user_in;
        char tmp;
        int y = -1;
        int x = -1;
        is_piece = false;

        // diaplay board
        display_board(board);

        // player 1 (user)
        while (!is_piece)
        {
            std::cout << "Where do you want to put your piece? ";
            std::cin >> tmp; // get letter
            y = (tmp - 'a');
            getline(std::cin, user_in);
            x = (std::stoi(user_in) - 1);
            // std::cout << "x = " << x << ", y = " << y << std::endl;
            is_piece = put_piece(board, x, y, 1);
            if (!is_piece)
                std::cout << "Wrong move, please try again!" << std::endl;
        }
        // add a save funciton.

        // check if player 1 won.
        // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        Player_1_bits = bits_calculator(board, 1);
        if (check.hasWon(Player_1_bits)) {
            std:: cout << "You won!" << std::endl;
            game_on = false;
        }
//-*
        // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
        // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

        // std::cout << std::endl << std::endl;

//*-/
//-*
        // AI turn
        if (game_on) {
            // Player_2_bits = bits_calculator(board, 2);

            std::pair<int, int> rsl = player_2.bestMove(board, Player_1_bits, Player_2_bits);// give board to best move
            put_piece(board, rsl.first, rsl.second, 2);
            Player_2_bits = bits_calculator(board, 2);
            // if (WinChecker(board, 2).result()) {
            if (check.hasWon(Player_2_bits)) {
                display_board(board);
                std:: cout << "Oh no, you lost!" << std::endl;
                game_on = false;
            }
        }
//*-/
    }

    player_2.scores.writeFile("./ML_files/random-file");
    return 0;
}

int Game::LaunchAIGame(AI &player_1) // maybe get oponing move from here.
{
    while (game_on)
    {

        Player_1_bits = bits_calculator(board, 1);
        if (check.hasWon(Player_1_bits)) {
            std:: cout << "You won!" << std::endl;
            game_on = false;
        }


        // AI 2 turn
        if (game_on) {
            std::pair<int, int> rsl = player_2.bestMove(board, Player_1_bits, Player_2_bits);

            put_piece(board, rsl.first, rsl.second, 2);

            Player_2_bits = bits_calculator(board, 2);

            if (check.hasWon(Player_2_bits)) {
                // display_board(board);
                std:: cout << "Oh no, you lost!" << std::endl;
                game_on = false; // maybe put a limit on the number of games ???
            }
        }


        if (game_on) {
            std::pair<int, int> rsl = player_1.bestMove(board, Player_2_bits, Player_1_bits);

            put_piece(board, rsl.first, rsl.second, 1);

            Player_1_bits = bits_calculator(board, 1);

            if (check.hasWon(Player_1_bits)) {
                // display_board(board);
                std:: cout << "You won!" << std::endl;
                game_on = false; // maybe put a limit on the number of games ???
            }
        }
    }
    return 0;
}

void Game::LaunchXAIGame(size_t X)
{
    AI player_1; // maybe separate the ml files between the two AIs, and combine them later.

    for (size_t i = 0; i < X; i++)
    {
        // reset board
        board.clear();
        board = std::vector<std::vector<char>>{15, std::vector<char>(15, 0)};

        // put first piece;
        board[i / 100][i % 100] = 1;

        // init AIs
        player_1.scores.readFile("./ML_files/random-file");
        player_2.scores.readFile("./ML_files/random-file");


        LaunchAIGame(player_1);

        // to know last good circular saves.
        std::cout << "Last tried = " << (i % 10) << std::endl;

        // merge Ais
        if (player_1.scores.scores.size() < player_2.scores.scores.size()) { // player_2 AI as more turn.
            for (size_t i = 0; i < player_1.scores.scores.size(); i++) {
                for (size_t j = 0; j < player_1.scores.scores[i].size(); j++) {
                    if (!player_2.scores.isElement(player_1.scores.scores[i][j].boardHash, i))
                        player_2.scores.scores[i].push_back(player_1.scores.scores[i][j]); // only add new elements.
                }
            }

            player_2.scores.writeFile("./ML_files/random-file");
            player_2.scores.writeFile("./ML_files/random-file" + std::to_string((i % 10))); // save file just in case
        }
        else {
            for (size_t i = 0; i < player_2.scores.scores.size(); i++) {
                for (size_t j = 0; j < player_2.scores.scores[i].size(); j++) {
                    if (!player_1.scores.isElement(player_2.scores.scores[i][j].boardHash, i))
                        player_1.scores.scores[i].push_back(player_2.scores.scores[i][j]); // only add new elements.
                }
            }

            player_1.scores.writeFile("./ML_files/random-file");
            player_1.scores.writeFile("./ML_files/random-file" + std::to_string((i % 10))); // save file just in case
        }

        // clear Ais
        player_1.scores.scores.clear();
        player_2.scores.scores.clear();
    }
}


void Game::display_board(std::vector<std::vector<char>> &board)
{
    std::cout << "   a  b  c  d  e  f  g  h  i  j  k  l  m  n  o" << std::endl;
    std::cout << "  =============================================" << std::endl;

    for (size_t i = 0; i < board.size(); i++) {
        std::string line = (((i + 1) < 10) ? " " : "") + std::to_string(i + 1) + "|";
        for (size_t j = 0; j < board[i].size(); j++) {
            line += (board[i][j] == 0) ? "-" : (board[i][j] == 1) ? "B" : "W";
            if (j != (board.size() - 1))
                line += "  ";
            else
                line += "|";
        }
        std::cout << line << std::endl;
    }
    std::cout << "  =============================================" << std::endl;
}

bool Game::put_piece(std::vector<std::vector<char>> &board, int &x, int &y, char piece)
{
    if ((x >= 0 and x < (int)board.size()) and (y >= 0 and y < (int)board.size()) and (board[x][y] == 0)) {
        board[x][y] = piece;
        return true;
    }
    return false;
}

uint256_t Game::bits_calculator(std::vector<std::vector<char>> &board, char piece)
{
    uint256_t rsl = 0;

    for (int i = (int)(board.size() - 1); i >= 0; i--) {
        for(int j = (int)(board[i].size() - 1); j >= 0; j--) {
            if (board[i][j] == piece)
                rsl = (rsl << 1) | 1;
            else
                rsl = (rsl << 1);
        }
    }
    return rsl;
}