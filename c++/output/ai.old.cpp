/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** ai
*/

#include "ai.hpp"
#include <chrono>

AI::AI(std::vector<std::vector<char>> board, char Ai_color, char Enemy_Color)
{
    _board = board;
    _AI_Color = Ai_color;
    _Enemy_Color = Enemy_Color;
}

AI::~AI()
{}

std::pair<int, int> AI::getNextMove()
{
    int bestScore = NINFINITY;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(_board);
    int y = 0;
    int x = 0;
    int best_y = 0;
    int best_x = 0;

    for (size_t i = 0; i < squares.size(); i++) {
        x = squares[i].first;
        y = squares[i].second;

        _board[x][y] = -1;
        int score = alphabeta(_board, 0, NINFINITY, INFINITY, false); // easy to multithread
        _board[x][y] = 0;
        if (score > bestScore) {
            // print("MAIS PUTAIN")
            bestScore = score;
            best_x = x;
            best_y = y;
        }
    }
    // ans = f'{chr(97 + best_y)} {best_x + 1}'
    // print(f'Returning {best_y} {best_x} for {bestScore}')
    return std::make_pair(best_x, best_y); // it's actually x then y.
    // return std::make_pair(best_y, best_x); // it's actually x then y.
}

std::vector<std::pair<int, int>> AI::getSquaresToCheck(std::vector<std::vector<char>> &my_board) // can be multithreaded.
{
    std::vector<std::pair<int, int>> adjacent;

    for (size_t i = 0; i < my_board.size(); i++) {
        for (size_t j = 0; j < my_board[i].size(); j++) {
            if (my_board[i][j] == 0 && isTouchingOccupied(i, j)) {
                // if (i == 0 && j == 1)
                //     int z = 0;
                adjacent.push_back(std::make_pair(i, j));
            }
        }
    }
    return adjacent;
}

bool AI::isTouchingOccupied(const int &x, const int &y) const
{
    return (occupied((x + 1), y) || occupied((x - 1), y) || occupied(x, (y + 1))
        || occupied(x, (y - 1)) || occupied((x + 1), (y + 1)) || occupied((x - 1), (y + 1))
        || occupied((x - 1), (y - 1)) || occupied((x + 1), (y - 1)));
}

bool AI::occupied(const int &x, const int &y) const
{
    if ((x >= 0 && x < (int) _board.size()) && (y >= 0 && y < (int) _board[0].size()))
        return _board[x][y];
    return false;
}


int AI::alphabeta(std::vector<std::vector<char>> &board, int depth, int alpha, int beta, bool isAiTurn)
{
    if (depth >= MAX_DEPTH) {
        // arrived at max depth, get the alphabeta values from this path and return
        return staticEval(board);
    }

    bool winner = WinChecker(board, (isAiTurn ? _AI_Color : _Enemy_Color)).result();
    if (winner) {
        // print(f'For, {"AI" if winner != 1 else "Human"}: ', end='')
        // print(f'CATASTROPHIC FAILURE IMMINENT for sit {matrix}: {winner}')
        // return 999999999 * (-1 if winner == 1 else 1)
        return 999999999 * (isAiTurn ? 1 : -1);
        // return 999999999 * (winner == 1 ? -1 : 1);
    }

    int best = (isAiTurn ? NINFINITY : INFINITY);
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(board);

    // std::cout << "sqrr = " << squares.size() << std::endl;
    // for (int i = 0; i < squares.size(); i++) {
    //     std::cout << "alphe_beta " << "x = " << squares[i].first << ", y = " << squares[i].second << std::endl;
    //     if (board[squares[i].first][squares[i].second] == 1 || board[squares[i].first][squares[i].second] == 2)
    //         printf("\n\nNOOOOP %d, %d\n\n", (int)squares[i].first, (int)squares[i].second);
    // }

    for (size_t i = 0; i < squares.size(); i++) {
        int x = squares[i].first;
        int y = squares[i].second;

        // if (board[y][x] == _Enemy_Color)
        //     pass

        // board[y][x] = (isAiTurn ? -1 : _Enemy_Color);//-1 if isAiTurn else 1
        board[x][y] = (isAiTurn ? -1 : _Enemy_Color);//-1 if isAiTurn else 1
        int score = alphabeta(board, depth + 1, alpha, beta, !isAiTurn);
        int best = (isAiTurn ? std::max(score, best) : std::min(score, best));

        if (isAiTurn)
            alpha = std::max(alpha, best);
        else
            beta = std::min(beta, best);

        // board[y][x] = 0;
        board[x][y] = 0;
        if (alpha >= beta)
            break;
    }
    return best;
}


int AI::staticEval(std::vector<std::vector<char>> &my_board) // one per thread
{
    int rsl = horizontalScore(my_board) + verticalScore(my_board) + diagonalScore(my_board);

    return rsl;
}

int AI::horizontalScore(std::vector<std::vector<char>> &my_board) // one per thread
{
    int score = 0;

    for (size_t i = 0; i < my_board.size(); i++) {
        char current = 0;
        size_t streak = 0;

        for (size_t j = 0; j < my_board[i].size(); j++) {
            scoreConsecutive(my_board[i][j], current, streak, score);
        }
        if (current != 0)
            score += current * adjacentBlockScore(streak);
    }
    return (-1 * score);
}

int AI::verticalScore(std::vector<std::vector<char>> &my_board) // one per thread
{
    int score = 0;

    for (size_t i = 0; i < my_board.size(); i++) {
        char current = 0;
        size_t streak = 0;

        for (size_t j = 0; j < my_board[i].size(); j++) {
            scoreConsecutive(my_board[j][i], current, streak, score);
        }
        if (current != 0)
            score += current * adjacentBlockScore(streak);
    }
    return -1 * score;
}

int AI::diagonalScore(std::vector<std::vector<char>> &my_board)// one per thread
{
    size_t lenght = my_board.size();
    int score = 0;
    std::vector<Score> res(4);
    int x = 0;
    int y = 0;

    for (size_t i = 4; i < lenght; i++) {
        for (size_t k = 0; k < res.size(); k++)
            res[k] = Score();
        for (size_t j = 0; j < i; j++) {
            x = (i - j);
            y = j;
            scoreConsecutive(my_board[x][y], res[0].current, res[0].streak, res[0].score);

            x = (lenght - 1 - j);
            y = (i - j);
            scoreConsecutive(my_board[x][y], res[1].current, res[1].streak, res[1].score);

            x = j;
            y = (lenght - 1 - i + j);
            scoreConsecutive(my_board[x][y], res[2].current, res[2].streak, res[2].score);

            x = (lenght - 1 - i);
            y = (lenght - 1 - j);
            scoreConsecutive(my_board[x][y], res[3].current, res[3].streak, res[3].score);
        }
        score += (res[0].score + res[1].score + res[2].score + res[3].score);
    }
    return (-1 * score);
}

void AI::scoreConsecutive(char &block, char &current, size_t &streak, int &score) // can be made better
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

int AI::adjacentBlockScore(const size_t &count) const
{
    std::array<int, 6> scoreMatrix{{0, 2, 4, 8, 16, 32}};

    return (count > scoreMatrix.size()) ? -1 : scoreMatrix[count];
}

extern "C" {
    bool ai(int **board) {
        std::vector<std::vector<char>> boardPP(15);

        for (int x = 0; x < 15; x++) {
            std::vector<char> tmp(15);
            for (int y = 0; y < 15; y++) {
                tmp[y] = board[x][y];
                // printf("%d", board[x][y]);
            }
            boardPP[x] = tmp;
        }

        // AI test(boardPP);
        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;

        bool battle = true;

        while (battle) {
            AI test_player_2(boardPP, 2, 1);
            begin = std::chrono::steady_clock::now();
            auto rsl2 = test_player_2.getNextMove();
            end = std::chrono::steady_clock::now();

            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

            std::cout << "Player 2" << " y = " << rsl2.first << ", z = " << rsl2.second << std::endl;
            boardPP[rsl2.first][rsl2.second] = 2;
            battle = !WinChecker(boardPP, 2).result();

            for (int x = 0; x < 15; x++) {
                for (int y = 0; y < 15; y++)
                    std::cout << (int) boardPP[x][y] << " ";
                std::cout << std::endl;
            }
            // battle = false;

            AI test_player_1(boardPP, 1, 2);
            begin = std::chrono::steady_clock::now();
            auto rsl1 = test_player_1.getNextMove();
            end = std::chrono::steady_clock::now();

            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

            std::cout << "Player 1" << " y = " << rsl1.first << ", z = " << rsl1.second << std::endl;
            boardPP[rsl1.first][rsl1.second] = 1;
            battle = !WinChecker(boardPP, 1).result();

            for (int x = 0; x < 15; x++) {
                for (int y = 0; y < 15; y++)
                    std::cout << (int) boardPP[x][y] << " ";
                std::cout << std::endl;
            }
            // battle = false;
        }



        int i = 0;

        //return WinChecker(boardPP, pieceNumber).result();
        return 0;
    }
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

    data[0][0] = 1;

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

    //std::cout << winCheck(1, data) << std::endl;
    //std::cout << true << std::endl;

    ai(data);

    for (int x = 0; x < 15; x++)
        delete[] data[x];

    delete[] data;


    return 0;
}