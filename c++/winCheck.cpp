#include <stdbool.h>
#include <stdio.h>

// #include <array>
#include <vector>
#include <iostream>

int countOccurrences(std::vector<char> arr, int x);
bool rowCheck(int pieceNumber, std::vector<std::vector<char>> boardPP);

std::vector<std::vector<char>> transposeDiagonalDec(std::vector<std::vector<char>> boardPP);
std::vector<char> getDiagonalDec(std::vector<std::vector<char>> boardPP, int digNum);

std::vector<std::vector<char>> transposeDiagonalInc(std::vector<std::vector<char>> boardPP);
std::vector<char> getDiagonalInc(std::vector<std::vector<char>> boardPP, int digNum);

std::vector<std::vector<char>> transpose(std::vector<std::vector<char>> &boardPP);
std::vector<char> getCol(std::vector<std::vector<char>> &boardPP, int colNum);


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

        // printf("\n\n");
        // printf("boardPP.size() = %ld\n", boardPP.size());

        // for (size_t x = 0; x < boardPP.size(); x++) {
        //     for (size_t y = 0; y < boardPP[x].size(); y++) {
        //         printf("%d", boardPP[x][y]);
        //     }
        // }

        // std::vector<std::vector<char>> boardPP;
        // // std::array<std::array<char, 15>, 15> boardPP;
        // for (int x = 0; x < 15; x++)
        //     for (int y = 0; y < 15; y++)
        //         boardPP[x][y] = board[x][y];
        // //         printf("%d", board[i][j]);
        // // return true;
        // // return rowCheck(pieceNumber, boardPP);
        // //if self.rowCheck(Piece_Number, self.board) or
        // // self.rowCheck(Piece_Number, self.transpose())or
        // // self.rowCheck(Piece_Number, self.transposeDiagonalInc()) or self.rowCheck(Piece_Number, self.transposeDiagonalDec()):
        return (
                    // rowCheck(pieceNumber, boardPP)
                /*|| */rowCheck(pieceNumber, transpose(boardPP))
                // || rowCheck(pieceNumber, transposeDiagonalInc(boardPP))
                // || rowCheck(pieceNumber, transposeDiagonalDec(boardPP))
                );
        return true;
    }
}

bool rowCheck(int pieceNumber, std::vector<std::vector<char>> boardPP) { // works
    // printf("\n\n\n C++:\n");
    // for (size_t i = 0; i < boardPP.size(); i++) {
    //     for (size_t j = 0; j < boardPP[i].size(); j++)
    //         printf("%d ", boardPP[i][j]);
    //     printf("\n");
    // }

    int connection;

    for (int x = 0; x < 15; x++) {
        // printf("\nC++: 1\n");
        if (countOccurrences(boardPP[x], pieceNumber)) {
            // printf("\nC++: 2\n");
            for (int z = 0; z < (15 - 3); z++) {
                connection = 0;
                for (int c = 0; c < 5; c++) {
                    if (boardPP[x][(z + c)] == pieceNumber)
                        connection++;
                    else
                        break;
                    if (connection == 5)
                        return true;
                }
            }
        }
    }
    return false;
}


std::vector<std::vector<char>> transposeDiagonalDec(std::vector<std::vector<char>> boardPP) {
    std::vector<std::vector<char>> lst;

    for (size_t i = 0; i < ((boardPP.size() * 2) - 1); i++)
        lst.push_back(getDiagonalDec(boardPP, i));
    return lst;
}

std::vector<char> getDiagonalDec(std::vector<std::vector<char>> boardPP, int digNum) {
    int index;
    std::vector<char> lst;

    if (digNum <= (int)(boardPP.size() - 1)) {
        index = (boardPP.size() - 1);
        for (int i = digNum; i >= 0; i--) {
            lst.push_back(boardPP[i][index]);
            index--;
        }
        return lst;
    }
    else {
        index = (((boardPP.size() * 2) - 2) - digNum);
        for (size_t i = (boardPP.size() - 1); i > (digNum - boardPP.size()); i--) {
            lst.push_back(boardPP[i][index]);
            index--;
        }
        return lst;
    }
}


std::vector<std::vector<char>> transposeDiagonalInc(std::vector<std::vector<char>> boardPP) {
    std::vector<std::vector<char>> lst;

    for (size_t i = 0; i < ((boardPP.size() * 2) - 1); i++)
        lst.push_back(getDiagonalInc(boardPP, i));
    return lst;
}

std::vector<char> getDiagonalInc(std::vector<std::vector<char>> boardPP, int digNum) {
    int index;
    std::vector<char> lst;

    if (digNum <= (int)(boardPP.size() - 1)) {
        index = (boardPP.size() - 1);
        for (int i = digNum; i >= 0; i--) {
            lst.push_back(boardPP[i][index]);
            index++;
        }
        return lst;
    }
    else {
        index = (digNum - (boardPP.size() + 1));
        for (size_t i = (boardPP.size() - 1); i > (digNum - boardPP.size()); i--) {
            lst.push_back(boardPP[i][index]);
            index--;
        }
        return lst;
    }
}


std::vector<std::vector<char>> transpose(std::vector<std::vector<char>> &boardPP) {
    std::vector<std::vector<char>> lst(15);
    // printf("\n\n\n C++:\n");

    for (size_t i = 0; i < boardPP.size(); i++)
        lst.push_back(getCol(boardPP, i));
    return lst;
}

std::vector<char> getCol(std::vector<std::vector<char>> &boardPP, int colNum) {
    std::vector<char> lst(15);
    // printf("\n\n\n C++:\n");

    for (size_t i = 0; i < boardPP.size(); i++) {
        // std::cout << "i = " << i << ", colNum = " << colNum << std::endl;
        // std::cout << "boardPP[i][colNum] = " << boardPP[i][colNum] << std::endl;
        lst.push_back(boardPP[i][colNum]);
    }
    return lst;
}