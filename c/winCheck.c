#include <stdbool.h>
#include <stdio.h>

// int main(int argc, char const *argv[])
// {
//     /* code */
//     return 0;
// }

bool winCheck(int **board)
{
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            printf("%d", board[i][j]);
    return true;
}