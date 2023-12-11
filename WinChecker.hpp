#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include <vector>
#include <iostream>

class WinChecker {
    public:
        WinChecker();
        ~WinChecker();
        static bool result(std::vector<std::vector<char>> &board, int pieceNumber);

    protected:
    private:
        static bool rowCheck(std::vector<std::vector<char>> board, int &pieceNumber);
        static int countOccurrences(std::vector<char> &arr, int &pieceNumber);

        static std::vector<std::vector<char>> transposeDiagonalDec(std::vector<std::vector<char>> &board);
        static std::vector<char> getDiagonalDec(std::vector<std::vector<char>> &board, int digNum);

        static std::vector<std::vector<char>> transposeDiagonalInc(std::vector<std::vector<char>> &board);
        static std::vector<char> getDiagonalInc(std::vector<std::vector<char>> &board, int digNum);

        static std::vector<std::vector<char>> transpose(std::vector<std::vector<char>> &board);
        static std::vector<char> getCol(std::vector<std::vector<char>> &board, int colNum);

        const static int _goal = 5;
};

#endif /* !GOMOKU_HPP */
