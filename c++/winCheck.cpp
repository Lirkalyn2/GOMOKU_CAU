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


        // transposeDiagonalInc(boardPP);

        return (rowCheck(pieceNumber, boardPP)
                || rowCheck(pieceNumber, transpose(boardPP))
                || rowCheck(pieceNumber, transposeDiagonalInc(boardPP))
                || rowCheck(pieceNumber, transposeDiagonalDec(boardPP))
                );
        return true;
    }
}

bool rowCheck(int pieceNumber, std::vector<std::vector<char>> boardPP) { // works
    int connection;

    for (int x = 0; x < 15; x++) {
        if (countOccurrences(boardPP[x], pieceNumber)) {
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


std::vector<std::vector<char>> transposeDiagonalDec(std::vector<std::vector<char>> boardPP) { // works
    std::vector<std::vector<char>> lst(((boardPP.size() * 2) - 1));//(15);

    for (size_t i = 0; i < ((boardPP.size() * 2) - 1); i++)
        // lst.push_back(getDiagonalDec(boardPP, i));//[i] = getDiagonalDec(boardPP, i);//.push_back(getDiagonalDec(boardPP, i));
        lst[i] = getDiagonalDec(boardPP, i);//.push_back(getDiagonalDec(boardPP, i));
    return lst;
}

std::vector<char> getDiagonalDec(std::vector<std::vector<char>> boardPP, int digNum) {
    int index;

    if (digNum <= (int)(boardPP.size() - 1)) {
        index = (boardPP.size() - 1);

        std::vector<char> lst(digNum + 1);

        for (int i = digNum; i >= 0; i--) {
            // lst.push_back(boardPP[i][index]);//[i] = boardPP[i][index];  //.push_back(boardPP[i][index]);
            lst[i] = boardPP[i][index];  //.push_back(boardPP[i][index]);
            index--;
        }
        return lst;
    }
    else {
        index = (((boardPP.size() * 2) - 2) - digNum);

        std::vector<char> lst(boardPP.size());

        for (size_t i = (boardPP.size() - 1); i > (digNum - boardPP.size()); i--) {
            // lst.push_back(boardPP[i][index]);//[i] = boardPP[i][index]; //.push_back(boardPP[i][index]);
            lst[i] = boardPP[i][index]; //.push_back(boardPP[i][index]);
            index--;
        }
        return lst;
    }
}


std::vector<std::vector<char>> transposeDiagonalInc(std::vector<std::vector<char>> boardPP) { // works
    std::vector<std::vector<char>> lst((boardPP.size() * 2) - 1);//(30);

    for (size_t i = 0; i < ((boardPP.size() * 2) - 1); i++)
        lst[i] = getDiagonalInc(boardPP, i); //.push_back(getDiagonalInc(boardPP, i));
    return lst;
}

std::vector<char> getDiagonalInc(std::vector<std::vector<char>> boardPP, int digNum) {
    int index;

    if (digNum <= (int)(boardPP.size() - 1)) {
        index = 0;

        std::vector<char> lst(digNum + 1);

        for (int i = digNum; i >= 0; i--) {
            // lst.push_back(boardPP[i][index]);//[i] = boardPP[i][index]; //.push_back(boardPP[i][index]);
            lst[i] = boardPP[i][index]; //.push_back(boardPP[i][index]);
            index++;
        }
        return lst;
    }
    else {
        index = (digNum - boardPP.size() + 1);

        std::vector<char> lst(boardPP.size());

        for (size_t i = (boardPP.size() - 1); i > (digNum - boardPP.size()); i--) {
            lst.push_back(boardPP[i][index]); //[i] = boardPP[i][index]; //.push_back(boardPP[i][index]);
            index++;
        }
        return lst;
    }
}


std::vector<std::vector<char>> transpose(std::vector<std::vector<char>> &boardPP) { // works, can be simplify.
    std::vector<std::vector<char>> lst(15);

    for (size_t i = 0; i < boardPP.size(); i++)
        lst[i] = getCol(boardPP, i);
    return lst;
}

std::vector<char> getCol(std::vector<std::vector<char>> &boardPP, int colNum) {
    std::vector<char> lst(15);

    for (size_t i = 0; i < boardPP.size(); i++) {
        lst[i] = boardPP[i][colNum];
    }
    return lst;
}

int main(void)
{
    int **data;

    data = new int*[15];

    for (int x = 0; x < 15; x++)
        data[x] = new int[15];

    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++)
            data[x][y] = 0;

/*
    data[0][0] = 1;
    data[0][1] = 1;
    data[0][2] = 1;
    data[0][3] = 1;
    data[0][4] = 1;
*/

/*
    data[0][0] = 1;
    data[1][0] = 1;
    data[2][0] = 1;
    data[3][0] = 1;
    data[4][0] = 1;
*/


/*
    data[0][4] = 1;
    data[1][3] = 1;
    data[2][2] = 1;
    data[3][1] = 1;
    data[4][0] = 1;
*/


/*
    data[0][0] = 1;
    data[1][1] = 1;
    data[2][2] = 1;
    data[3][3] = 1;
    data[4][4] = 1;
*/

    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 15; y++)
            std::cout << data[x][y] << " ";
        std::cout << std::endl;
    }

    std::cout << winCheck(1, data) << std::endl;
    std::cout << true << std::endl;
    return 0;
}
