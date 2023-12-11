#include "WinChecker.hpp"


WinChecker::WinChecker() {}

WinChecker::~WinChecker() {}


bool WinChecker::rowCheck(std::vector<std::vector<char>> board, int &pieceNumber)
{
    int connection;

    for (std::size_t x = 0; x < board.size(); x++)
        if (countOccurrences(board[x], pieceNumber))
            for (std::size_t z = 0; z < (board.size() - 3); z++) {
                connection = 0;
                for (int c = 0; ((c < _goal) && ((z + c) < board[x].size())); c++) {
                    if (board[x][(z + c)] == pieceNumber)
                        connection++;
                    else
                        break;
                    if (connection == _goal)
                        return true;
                }
            }
    return false;
}

int WinChecker::countOccurrences(std::vector<char> &arr, int &pieceNumber)
{
    int rsl = 0;

    for (std::size_t i = 0; i < arr.size(); i++)
        if (arr[i] == pieceNumber)
            rsl++;
    return rsl;
}

std::vector<std::vector<char>> WinChecker::transposeDiagonalDec(std::vector<std::vector<char>> &board)
{
    std::vector<std::vector<char>> lst(((board.size() * 2) - 1));

    for (std::size_t i = 0; i < ((board.size() * 2) - 1); i++)
        lst[i] = getDiagonalDec(board, i);
    return lst;
}

std::vector<char> WinChecker::getDiagonalDec(std::vector<std::vector<char>> &board, int digNum) {
    int index;

    if (digNum <= (int)(board.size() - 1)) {
        index = (board.size() - 1);

        std::vector<char> lst(digNum + 1);

        for (int i = digNum; i >= 0; i--) {
            lst[i] = board[i][index];
            index--;
        }
        return lst;
    }
    else {
        index = (((board.size() * 2) - 2) - digNum);

        std::vector<char> lst(board.size());

        for (std::size_t i = (board.size() - 1); i > (digNum - board.size()); i--) {
            lst[i] = board[i][index];
            index--;
        }
        return lst;
    }
}

std::vector<std::vector<char>> WinChecker::transposeDiagonalInc(std::vector<std::vector<char>> &board)
{
    std::vector<std::vector<char>> lst((board.size() * 2) - 1);

    for (std::size_t i = 0; i < ((board.size() * 2) - 1); i++)
        lst[i] = getDiagonalInc(board, i);
    return lst;
}

std::vector<char> WinChecker::getDiagonalInc(std::vector<std::vector<char>> &board, int digNum)
{
    int index;

    if (digNum <= (int)(board.size() - 1)) {
        index = 0;

        std::vector<char> lst(digNum + 1);

        for (int i = digNum; i >= 0; i--) {
            lst[i] = board[i][index];
            index++;
        }
        return lst;
    }
    else {
        index = (digNum - board.size() + 1);

        std::vector<char> lst(board.size());

        for (std::size_t i = (board.size() - 1); i > (digNum - board.size()); i--) {
            index++;
        }
        return lst;
    }
}

std::vector<std::vector<char>> WinChecker::transpose(std::vector<std::vector<char>> &board)
{
    std::vector<std::vector<char>> lst(board.size());

    for (std::size_t i = 0; i < board.size(); i++)
        lst[i] = getCol(board, i);
    return lst;
}

std::vector<char> WinChecker::getCol(std::vector<std::vector<char>> &board, int colNum)
{
    std::vector<char> lst(board.size());

    for (std::size_t i = 0; i < board.size(); i++) {
        lst[i] = board[i][colNum];
    }
    return lst;
}

bool WinChecker::result(std::vector<std::vector<char>> &board, int pieceNumber)
{
    return (rowCheck(board, pieceNumber) || rowCheck(transpose(board), pieceNumber) || rowCheck(transposeDiagonalInc(board), pieceNumber) || rowCheck(transposeDiagonalDec(board), pieceNumber));
}