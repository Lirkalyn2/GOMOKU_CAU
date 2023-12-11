#include <string>
#include <vector>
#include <iostream>

#include "WinChecker.hpp"
#include "ai.hpp"

void display_board(std::vector<std::vector<char>> &board)
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

bool put_piece(std::vector<std::vector<char>> &board, int &x, int &y, char piece)
{
    if ((x >= 0 and x < board.size()) and (y >= 0 and y < board.size()) and (board[x][y] == 0)) {
        board[x][y] == piece;
        return true;
    }
    return false;
}

int main(int argc, char const *argv[])
{
    bool game_on = true;
    std::vector<std::vector<char>> board{15, std::vector<char>(15, 0)};
    uint256_t Player_1_bits = 0;
    uint256_t Player_2_bits = 0;


    while (game_on)
    {
        std::string user_in;
        char tmp;
        int x = -1;
        int y = -1;
        bool is_piece = false;

        display_board(board);

        while (!is_piece)
        {
            std::cout << "Where do you want to put your piece? ";
            std::cin >> tmp; // get letter
            x = (tmp - 'a');
            std::cin >> tmp; // get numnber
            y = (tmp - '0' - 1);
            std::cout << "x = " << x << ", y = " << y << std::endl;
            is_piece = put_piece(board, x, y, 1);
            if (!is_piece)
                std::cout << "Wrong move, please try again!" << std::endl;
        }

        if (WinChecker(board, 1).result()) {
            std:: cout << "You won!" << std::endl;
            game_on = false;
        }


        Player_1_bits = 0;
        Player_2_bits = 0;

        for (int i = (int)(board.size() - 1); i >= 0; i--) {
            for(int j = (int)(board[i].size() - 1); j >= 0; j--) {

                if (board[i][j] == 1)
                    Player_1_bits = (Player_1_bits << 1) | 1;
                else
                    Player_1_bits = (Player_1_bits << 1);

                if (board[i][j] == 2)
                    Player_2_bits = (Player_2_bits << 1) | 1;
                else
                    Player_2_bits = (Player_2_bits << 1);
            }
        }


        AI test_player_2(board, 2, 1);
        std::pair<int, int> rsl = test_player_2.bestMove(Player_1_bits, Player_2_bits);
        put_piece(board, rsl.first, rsl.second, 2);

        // // for (size_t i = 0; i < board.size(); i++) {
        // //     for (size_t j = 0; j < board[i].size(); j++)
        // //         std::cout << (int)board[i][j] << " ";
        // //     std::cout << std::endl;
        // // }
        // display_board(board);
        // game_on = false;
    }


    return 0;
}
