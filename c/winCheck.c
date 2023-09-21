#include <stdbool.h>
#include <stdio.h>

int countOccurrences(int *arr, int n, int x);
bool rowCheck(int pieceNumber, int **board);


bool winCheck(int pieceNumber, int **board) {
    // for (int i = 0; i < 15; i++)
    //     for (int j = 0; j < 15; j++)
    //         printf("%d", board[i][j]);
    // return true;
    return rowCheck(pieceNumber, board);
}

bool rowCheck(int pieceNumber, int **board) {
    int connection;

    for (int x = 0; x < 15; x++)
        if (countOccurrences(board[x], 15, pieceNumber))
            for (int z = 0; z < (15 - 3); z++) {
                connection = 0;
                for (int c = 0; c < 5; c++) {
                    if (board[x][(z + c)] == pieceNumber)
                        connection++;
                    else
                        break;
                    if (connection == 5)
                        return true;
                }
            }
    return false;
}

int **transposeDiagonalDec() {

}

int *getDiagonalDec(int **localBoard, int digNum) {

}