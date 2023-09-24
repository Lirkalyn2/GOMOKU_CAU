#include "WinChecker.hpp"


WinChecker::WinChecker(std::vector<std::vector<char>> board, int pieceNumber)
{
    _board = board;
    _pieceNumber = pieceNumber;
}

WinChecker::~WinChecker() {}


bool WinChecker::rowCheck(std::vector<std::vector<char>> board)
{
    int connection;

    for (std::size_t x = 0; x < board.size(); x++)
        if (countOccurrences(board[x]))
            for (std::size_t z = 0; z < (board.size() - 3); z++) {
                connection = 0;
                for (int c = 0; ((c < _goal) && ((z + c) < board[x].size())); c++) {
                    if (board[x][(z + c)] == _pieceNumber)
                        connection++;
                    else
                        break;
                    if (connection == _goal)
                        return true;
                }
            }
    return false;
}

int WinChecker::countOccurrences(std::vector<char> arr)
{
    int rsl = 0;

    for (std::size_t i = 0; i < arr.size(); i++)
        if (arr[i] == _pieceNumber)
            rsl++;
    return rsl;
}

std::vector<std::vector<char>> WinChecker::transposeDiagonalDec()
{
    std::vector<std::vector<char>> lst(((_board.size() * 2) - 1));

    for (std::size_t i = 0; i < ((_board.size() * 2) - 1); i++)
        lst[i] = getDiagonalDec(i);
    return lst;
}

std::vector<char> WinChecker::getDiagonalDec(int digNum) {
    int index;

    if (digNum <= (int)(_board.size() - 1)) {
        index = (_board.size() - 1);

        std::vector<char> lst(digNum + 1);

        for (int i = digNum; i >= 0; i--) {
            lst[i] = _board[i][index];
            index--;
        }
        return lst;
    }
    else {
        index = (((_board.size() * 2) - 2) - digNum);

        std::vector<char> lst(_board.size());

        for (std::size_t i = (_board.size() - 1); i > (digNum - _board.size()); i--) {
            lst[i] = _board[i][index];
            index--;
        }
        return lst;
    }
}

std::vector<std::vector<char>> WinChecker::transposeDiagonalInc()
{
    std::vector<std::vector<char>> lst((_board.size() * 2) - 1);

    for (std::size_t i = 0; i < ((_board.size() * 2) - 1); i++)
        lst[i] = getDiagonalInc(i);
    return lst;
}

std::vector<char> WinChecker::getDiagonalInc(int digNum)
{
    int index;

    if (digNum <= (int)(_board.size() - 1)) {
        index = 0;

        std::vector<char> lst(digNum + 1);

        for (int i = digNum; i >= 0; i--) {
            lst[i] = _board[i][index];
            index++;
        }
        return lst;
    }
    else {
        index = (digNum - _board.size() + 1);

        std::vector<char> lst(_board.size());

        for (std::size_t i = (_board.size() - 1); i > (digNum - _board.size()); i--) {
            index++;
        }
        return lst;
    }
}


std::vector<std::vector<char>> WinChecker::transpose()
{
    std::vector<std::vector<char>> lst(_board.size());

    for (std::size_t i = 0; i < _board.size(); i++)
        lst[i] = getCol(i);
    return lst;
}

std::vector<char> WinChecker::getCol(int colNum)
{
    std::vector<char> lst(_board.size());

    for (std::size_t i = 0; i < _board.size(); i++) {
        lst[i] = _board[i][colNum];
    }
    return lst;
}

bool WinChecker::result()
{
    return (rowCheck(_board) || rowCheck(transpose()) || rowCheck(transposeDiagonalInc()) || rowCheck(transposeDiagonalDec()));
}


extern "C" {
    bool winCheck(int pieceNumber, int **board) {
        std::vector<std::vector<char>> boardPP(15);

        for (int x = 0; x < 15; x++) {
            std::vector<char> tmp(15);
            for (int y = 0; y < 15; y++) {
                tmp[y] = board[x][y];
                // printf("%d", board[x][y]);
            }
            boardPP[x] = tmp;
        }

        return WinChecker(boardPP, pieceNumber).result();
    }
}

// int main(void)
// {
//     int **data;

//     data = new int*[15];

//     for (int x = 0; x < 15; x++)
//         data[x] = new int[15];

//     for (int x = 0; x < 15; x++)
//         for (int y = 0; y < 15; y++)
//             data[x][y] = 0;

// /*
//     data[0][0] = 1;
//     data[0][1] = 1;
//     data[0][2] = 1;
//     data[0][3] = 1;
//     data[0][4] = 1;
// */

// /*
//     data[0][0] = 1;
//     data[1][0] = 1;
//     data[2][0] = 1;
//     data[3][0] = 1;
//     data[4][0] = 1;
// */


// /*
//     data[0][4] = 1;
//     data[1][3] = 1;
//     data[2][2] = 1;
//     data[3][1] = 1;
//     data[4][0] = 1;
// */


// /*
//     data[0][0] = 1;
//     data[1][1] = 1;
//     data[2][2] = 1;
//     data[3][3] = 1;
//     data[4][4] = 1;
// */

//     for (int x = 0; x < 15; x++) {
//         for (int y = 0; y < 15; y++)
//             std::cout << data[x][y] << " ";
//         std::cout << std::endl;
//     }

//     std::cout << winCheck(1, data) << std::endl;
//     std::cout << true << std::endl;


//     for (int x = 0; x < 15; x++)
//         delete[] data[x];

//     delete[] data;


//     return 0;
// }
