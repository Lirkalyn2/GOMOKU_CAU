#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include <vector>
#include <iostream>

class WinChecker {
    public:
        WinChecker(std::vector<std::vector<char>> board, int pieceNumber);
        ~WinChecker();

        bool rowCheck(std::vector<std::vector<char>> board);
        int countOccurrences(std::vector<char> arr);

        std::vector<std::vector<char>> transposeDiagonalDec();
        std::vector<char> getDiagonalDec(int digNum);

        std::vector<std::vector<char>> transposeDiagonalInc();
        std::vector<char> getDiagonalInc(int digNum);

        std::vector<std::vector<char>> transpose();
        std::vector<char> getCol(int colNum);

        bool result();

    protected:
    private:
        std::vector<std::vector<char>> _board;
        char _pieceNumber;
        const static int _goal = 5;
};

#endif /* !GOMOKU_HPP */
