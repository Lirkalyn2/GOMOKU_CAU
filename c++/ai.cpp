
#include <vector>
#include <array>
#include <tuple>
#include <iostream>
#include "Score.hpp"
#include "WinChecker.hpp"

std::pair<int, int> getNextMove(std::vector<std::vector<char>> &board);
std::vector<std::pair<int, int>> getSquaresToCheck(std::vector<std::vector<char>> &board); // can be multithreaded.
bool isTouchingOccupied(std::vector<std::vector<char>> &board, int &x, int &y);
bool occupied(std::vector<std::vector<char>> &board, int x, int y);
int alphabeta(std::vector<std::vector<char>> &board, int depth, int alpha, int beta, bool isAiTurn);
int staticEval(std::vector<std::vector<char>> &board);
int horizontalScore(std::vector<std::vector<char>> &board);
int verticalScore(std::vector<std::vector<char>> &board);
int diagonalScore(std::vector<std::vector<char>> &board);
void scoreConsecutive(char &block, int &current, int &streak, int &score); // can be made better
int adjacentBlockScore(int &count);


int MAX_DEPTH = 4;
int Infinity = 100000;
int NInfinity = -100000;
int move = 0;

std::pair<int, int> getNextMove(std::vector<std::vector<char>> &board)
{
    int bestScore = NInfinity;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(board);
    int y = 0;
    int x = 0;
    int best_y = 0;
    int best_x = 0;

    // std::cout << squares << std::endl;
    for (int i = 0; i < squares.size(); i++) {
        x = squares[i].first;
        y = squares[i].second;

        board[x][y] = -1;
        int score = alphabeta(board, 0, NInfinity, Infinity, false);
        board[x][y] = -0;
        // print(f'{[x, y]}`s bestscore of {score} evaluated to best {bestScore}')
        if (score > bestScore) {
            // print("MAIS PUTAIN")
            bestScore = score;
            best_x = x;
            best_y = y;
        }
    }
    // ans = f'{chr(97 + best_y)} {best_x + 1}'
    // print(f'Returning {best_y} {best_x} for {bestScore}')
    return std::make_pair(best_y, best_x);
}

std::vector<std::pair<int, int>> getSquaresToCheck(std::vector<std::vector<char>> &board) // can be multithreaded.
{
    std::vector<std::pair<int, int>> adjacent;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == 0 && isTouchingOccupied(board, i, j))
                adjacent.push_back(std::make_pair(i, j));
        }
    }
    return adjacent;
}

bool isTouchingOccupied(std::vector<std::vector<char>> &board, int &x, int &y)
{
    return(occupied(board, (x + 1), y) || occupied(board, (x - 1), y) || occupied(board, x, (y + 1))
        || occupied(board, x, (y - 1)) || occupied(board, (x + 1), (y + 1)) || occupied(board, (x - 1), (y + 1))
        || occupied(board, (x - 1), (y - 1)) || occupied(board, (x + 1), (y - 1)));
}

bool occupied(std::vector<std::vector<char>> &board, int x, int y)
{
    if ((x >= 0 && x <= board.size()) && (y >= 0 && y <= board[0].size()))
        return board[x][y];
    return false;
}

int alphabeta(std::vector<std::vector<char>> &board, int depth, int alpha, int beta, bool isAiTurn)
{
    if (depth >= MAX_DEPTH) {
        // arrived at max depth, get the alphabeta values from this path and return
        // int eval = staticEval(board);
        // return eval;
        return staticEval(board);
    }

    bool winner = WinChecker(board, (isAiTurn ? 2 : 1)).result();
    if (winner) {
        // print(f'For, {"AI" if winner != 1 else "Human"}: ', end='')
        // print(f'CATASTROPHIC FAILURE IMMINENT for sit {matrix}: {winner}')
        // return 999999999 * (-1 if winner == 1 else 1)
        return 999999999 * (isAiTurn ? 1 : -1);//(-1 if winner == 1 else 1);
    }

    int best = (isAiTurn ? NInfinity : Infinity);
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(board);

    for (int i = 0; i < squares.size(); i++) {
        int x = squares[i].first;
        int y = squares[i].second;

        board[y][x] = (isAiTurn ? -1 : 1);//-1 if isAiTurn else 1
        int score = alphabeta(board, depth + 1, alpha, beta, !isAiTurn);
        // best = max(score, best) if isAiTurn else min(score, best)
        int best = (isAiTurn ? std::max(score, best) : std::min(score, best));

        if (isAiTurn)
            alpha = std::max(alpha, best);
        else
            beta = std::min(beta, best);

        board[y][x] = 0;
        if (alpha >= beta)
            break;
    }
    return best;
}


int staticEval(std::vector<std::vector<char>> &board)
{
    int rsl = horizontalScore(board) + verticalScore(board) + diagonalScore(board);

    return rsl;
}

int horizontalScore(std::vector<std::vector<char>> &board)
{
    int score = 0;

    for (int i = 0; i < board.size(); i++) {
        int current = 0;
        int streak = 0;

        for (int j = 0; j < board[i].size(); j++) {
            // (current, streak, score) = scoreConsecutive(matrix[i][j], current, streak, score);
            scoreConsecutive(board[i][j], current, streak, score); // need to convert board[i][j]
        }
        if (current != 0)
            score += current * adjacentBlockScore(streak);
    }
    return -1 * score;
}

int verticalScore(std::vector<std::vector<char>> &board)
{
    int score = 0;

    for (int i = 0; i < board.size(); i++) {
        int current = 0;
        int streak = 0;

        for (int j = 0; j < board[i].size(); j++) {
            // (current, streak, score) = scoreConsecutive(matrix[i][j], current, streak, score);
            scoreConsecutive(board[j][i], current, streak, score); // need to convert board[i][j]
        }
        if (current != 0)
            score += current * adjacentBlockScore(streak);
    }
    return -1 * score;
}

int diagonalScore(std::vector<std::vector<char>> &board)
{
    size_t lenght = board.size();
    int score = 0;
    std::vector<Score> res(4);
    int x = 0;
    int y = 0;

    for (int i = 4; i < lenght; i++) {
        for (int k = 0; k < res.size(); k++)
            res[k] = Score();
        for (int j = 0; j < i; j++) {
            x = (i - j);
            y = j;
            scoreConsecutive(board[x][y], res[0].current, res[0].streak, res[0].score); //process(matrix[x][y], res["d1"]);
            // std::swap(res[0].current, res[0].streak);

            x = (lenght - 1 - j);
            y = (i - j);
            scoreConsecutive(board[x][y], res[1].current, res[1].streak, res[1].score); //process(matrix[x][y], res["d1"]);
            // std::swap(res[1].current, res[1].streak);

            x = j;
            y = (lenght - 1 - i + j);
            scoreConsecutive(board[x][y], res[2].current, res[2].streak, res[2].score); //process(matrix[x][y], res["d1"]);
            // std::swap(res[2].current, res[2].streak);

            x = (lenght - 1 - i);
            y = (lenght - 1 - j);
            scoreConsecutive(board[x][y], res[3].current, res[3].streak, res[3].score); //process(matrix[x][y], res["d1"]);
            // std::swap(res[3].current, res[3].streak);
        }
        score += (res[0].score + res[1].score + res[2].score + res[3].score);
        // return (-1 * score);
    }
    // the return should propably be here!!!
    return (-1 * score);
}

// std::tuple<int, int, int> scoreConsecutive(int &block, int &current, int &streak, int &score) // can be made better
// (int, int ,int)scoreConsecutive(int &block, int &current, int &streak, int &score) // can be made better
// {
//     if (block != current) {
//         if (current == 0) {
//             current = block;
//             streak = 1;
//         }
//         else {
//             score += (current * adjacentBlockScore(streak));
//             current = block;
//             streak = 1;
//         }
//     }
//     else {
//         if (block != 0)
//             streak += 1;
//     }

//     return {current, streak, score};
// }

void scoreConsecutive(char &block, int &current, int &streak, int &score) // can be made better
{
    if (block != current) {
        if (current == 0) {
            current = block;
            streak = 1;
        }
        else {
            score += (current * adjacentBlockScore(streak));
            current = block;
            streak = 1;
        }
    }
    else {
        if (block != 0)
            streak += 1;
    }
}

int adjacentBlockScore(int &count)
{
    std::array<int, 6> scoreMatrix{{0, 2, 4, 8, 16, 32}};

    return (count < 0 || count > scoreMatrix.size()) ? -1 : scoreMatrix[count];
}