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

std::pair<int, int> AI::bestMove(uint256_t humanBits, uint256_t cpuBits)
{
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(_board);
    std::pair<int, int> move;
    int alpha = NINFINITY;
    int beta = INFINITY;
    int bestScore = NINFINITY;

    for (size_t i = 0; i < squares.size(); i++) {
        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = uint256_t((y * 15) + x);

        _board[y][x] = -1;
        cpuBits |= 1 << pos;
        int score = alphabeta(_board, 1, alpha, beta, false, humanBits, cpuBits);

        std::cout << "Final score for X:" << x << " et Y:" << y << " score:" << score << " Fore evaluation : " << staticEval(humanBits) << " evel" << staticEval(cpuBits) << std::endl;

        _board[y][x] = 0;
        cpuBits &= ~(1 << pos);

        // if we find a win, play it immediately
        if(score == 9999){
            return std::make_pair(y, x);
        }

        if(score > bestScore){
            alpha = score;
            bestScore = score;
            move = std::make_pair(y, x);
        }
    }
    return move;
}

std::vector<std::pair<int, int>> AI::getSquaresToCheck(const std::vector<std::vector<char>> &my_board)
{
    std::vector<std::pair<int, int>> rsl;
    std::vector<char> tmp_rsl;

    for (size_t i = 0; i < my_board.size(); i++) {
        for (size_t j = 0; j < my_board[i].size(); j++) {
            if (my_board[j][i] != 0) {
                addAdjacent(i, j, tmp_rsl, my_board);
            }
        }
    }


    for (size_t i = 0; i < tmp_rsl.size(); i++) {
        // std::cout << "tmp_rsl[" << i << "] = " << (int)tmp_rsl[i] << std::endl;
        int y = (tmp_rsl[i] & 0x0f);
        int x = (tmp_rsl[i] >> 4);
        if (my_board[y][x] <= 0) // band aid
            rsl.push_back(std::make_pair(y, x));
            // rsl.push_back(std::make_pair((tmp_rsl[i] & 0x0f), (tmp_rsl[i] >> 4)));
        // rsl.push_back(std::make_pair((tmp_rsl[i] >> 4), (tmp_rsl[i] & 0x0f)));
    }
    return rsl;
}

void AI::addAdjacent(const char i, const char j, std::vector<char> &list, const std::vector<std::vector<char>> &my_board) // can be multi threaded
{
    put((i + 1), j, list, my_board);
    put((i - 1), j, list, my_board);
    put(i, (j + 1), list, my_board);
    put(i, (j - 1), list, my_board);
    put((i + 1), (j + 1), list, my_board);
    put((i - 1), (j + 1), list, my_board);
    put((i - 1), (j - 1), list, my_board);
    put((i + 1), (j - 1), list, my_board);
}

void AI::put(const char y, const char x, std::vector<char> &list, const std::vector<std::vector<char>> &my_board)
{
    char combination = ((y << 4) | x);
    for(int i = 0; i < list.size(); i++)
        if (list[i] == combination)
            return;

    if ((x >= 0 && x <= (char)(my_board.size() - 2)) && (y >= 0 && y <= (char)(my_board.size() - 2)) && (my_board[y][x] == 0))
        list.push_back(combination);
}

int AI::alphabeta(std::vector<std::vector<char>> matrix, int depth, int alpha, int beta, bool isAiTurn, uint256_t playerBits, uint256_t opponentBits)
{

    if (checkWinner(opponentBits, depth)) {
//        std::cout << "Critical defeat! And a " << (isAiTurn ? " bad" : "good") << " one!!!" << std::endl;
        return isAiTurn ? -9999 : 9999;
    }

    // stop at MAX_DEPTH
    if (depth >= MAX_DEPTH) {
        if (checkWinner(playerBits, depth)) {
            return isAiTurn ? 9999 : -9999;
        } else {
            size_t eval = staticEval(playerBits) - staticEval(opponentBits);
            return isAiTurn ? eval : -eval;
        }
    }

    int best = isAiTurn ? -9999 : 9999;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(matrix);//, depth);

    for (size_t i = 0; i < squares.size(); i++) {

        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = ((y * 15) + x);

        matrix[y][x] = (isAiTurn ? -1 : 1);
        playerBits |= 1 << pos;

        int score = alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);

        matrix[y][x] = 0;
        playerBits &= ~(1 << pos);

        std::cout << "For X:" << x << " et Y:" << y << " score:" << score << " for " << (isAiTurn ? "us" : "human") << std::endl;
        if(isAiTurn){
            if (score >= beta) {
                return score;
            }

            best = std::max(score, best);
            alpha = std::max(alpha, score);
        } else {
            if (score <= alpha) {
                return score;
            }

            best = std::min(score, best);
            beta = std::min(beta, score);
        }

        if (score == 9999 && isAiTurn) {
            return score;
        }
        else if (score == -9999 && !isAiTurn) {
            return score;
        }
    }
    return best;
}

// bool AI::checkWinner(uint256_t &matrix, uint256_t &bits, int &depth)
bool AI::checkWinner(uint256_t &bits, int &depth)
{
    // if(this.totalMoves + depth < 9){
    //     return false;
    // }

    // if(checkCache('winners', bits) !== false){
    //     return checkCache('winners', bits);
    // }

    if(hasWon(bits)){
        // putCache('winners', bits, true);
        return true;
    }

    // putCache('winners', bits, false);
    return false;
}

bool AI::hasWon(const uint256_t &matrix)
{
    uint256_t h = 31;
    uint256_t v = 1152956690052710401;
    // uint256_t d1 = 18447025552981295105;
    uint256_t d1 = 281479271743489;
    d1 *= 65536;
    d1 += 1;
    uint256_t d2 = 1152991877646254096;

    if(matchBitmask(matrix, h)) return true;
    if(matchBitmask(matrix, d1)) return true;
    if(matchBitmask(matrix, d2)) return true;

    // vertical is a "bit" different :)
    while(v <= matrix) {
        if((v & matrix) == v) return true;
        v *= 2;
    }

    return false;
}

bool AI::matchBitmask(const uint256_t &matrix, uint256_t &mask)
{
    for(size_t i = 0; mask <= matrix; i++, mask *= 2){
        if((i % 15) > 10) continue;
        if((mask & matrix) == mask) return true;
    }

    return false;
}

size_t calcStreak(int streak) {
    size_t pointArray[6] = {0, 2, 8, 64, 512, 9999};
    return (pointArray[streak]);
}


size_t calcVertical(uint256_t vMask, const uint256_t &matrix) {
    int streak = 0;
    int total = 0;

    while (vMask <= matrix) {
        if ((vMask & matrix) == vMask) {
            streak++;
            total += calcStreak(streak);
        } else {
            streak = 0;
        }
        vMask *= 32768;
    }
    return total;
}

size_t AI::staticEval(const uint256_t &matrix)
{

    size_t total = 0;
    uint256_t hMask = 3;
    uint256_t vMask = 32769;
    uint256_t d1Mask = 65537;
    uint256_t d2Mask = 16385;

    total += matchMask(hMask, matrix);
    total += matchMask(d1Mask, matrix);
    total += matchMask(d2Mask, matrix);

    for (int i = 0; i < 15; i++, vMask *= 2) {
        total += calcVertical(vMask, matrix);
    }

    return total;
}

size_t AI::matchMask(uint256_t &mask, const uint256_t &matrix)
{
    size_t count = 0;
    int streak = 0;

    for(size_t i = 0; mask <= matrix; i++, mask *= 2){
        if((i%15) > 13) continue;
        if((mask & matrix) == mask) {
            streak++;
            count += calcStreak(streak);
        } else {
            streak = 0;
        }
    }

    return count;
}

extern "C" {
    int ai(int **board) {
        std::vector<std::vector<char>> boardPP(15);

        for (int x = 0; x < 15; x++) {
            std::vector<char> tmp(15);
            for (int y = 0; y < 15; y++) {
                tmp[y] = board[x][y];
            }
            boardPP[x] = tmp;
        }

        uint256_t Player_1_bits = 0;
        uint256_t Player_2_bits = 0;

        for (int i = (int)(boardPP.size() - 1); i >= 0; i--) {
            for(int j = (int)(boardPP[i].size() - 1); j >= 0; j--) {

                if (boardPP[i][j] == 1)
                    Player_1_bits = (Player_1_bits << 1) | 1;
                else
                    Player_1_bits = (Player_1_bits << 1);

                if (boardPP[i][j] == 2)
                    Player_2_bits = (Player_2_bits << 1) | 1;
                else
                    Player_2_bits = (Player_2_bits << 1);
            }
        }

        AI test_player_2(boardPP, 2, 1);
        std::pair<int, int> rsl = test_player_2.bestMove(Player_1_bits, Player_2_bits);


        return ((rsl.first * 100) + rsl.second);
        // return std::to_string(rsl.first) + " " + std::to_string(rsl.second);

        // return std::string("ok").c_str();

        // return WinChecker(boardPP, pieceNumber).result();
        // return std::string();
    }
}

// extern "C" {
//     bool ai(int **board) {
//         std::vector<std::vector<char>> boardPP(15);

//         for (int x = 0; x < 15; x++) {
//             std::vector<char> tmp(15);
//             for (int y = 0; y < 15; y++) {
//                 tmp[y] = board[x][y];
//                 // printf("%d", board[x][y]);
//             }
//             boardPP[x] = tmp;
//         }

//         // AI test(boardPP);
//         std::chrono::steady_clock::time_point begin;
//         std::chrono::steady_clock::time_point end;

//         bool battle = true;
//         uint256_t Player_1_bits = 0;
//         uint256_t Player_2_bits = 0;

//         while (battle) {

//             Player_1_bits = 0;
//             Player_2_bits = 0;
//             for (int i = (int)(boardPP.size() - 1); i >= 0; i--) {
//                 for(int j = (int)(boardPP[i].size() - 1); j >= 0; j--) {

//                     if (boardPP[i][j] == 1)
//                         Player_1_bits = (Player_1_bits << 1) | 1;
//                     else
//                         Player_1_bits = (Player_1_bits << 1);

//                     if (boardPP[i][j] == 2)
//                         Player_2_bits = (Player_2_bits << 1) | 1;
//                     else
//                         Player_2_bits = (Player_2_bits << 1);
//                 }
//             }
//             std::cout << "Player_1_bits = " << Player_1_bits << std::endl;
//             std::cout << "Player_2_bits = " << Player_2_bits << std::endl;



//             AI test_player_2(boardPP, 2, 1);
//             begin = std::chrono::steady_clock::now();
//             auto rsl2 = test_player_2.bestMove(Player_1_bits, Player_2_bits);
//             end = std::chrono::steady_clock::now();

//             std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
//             std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//             std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

//             std::cout << "Player 2" << " y = " << rsl2.first << ", z = " << rsl2.second << std::endl;
//             boardPP[rsl2.first][rsl2.second] = 2;
//             battle = !WinChecker(boardPP, 2).result();

//             for (int x = 0; x < 15; x++) {
//                 for (int y = 0; y < 15; y++)
//                     std::cout << (int) boardPP[x][y] << " ";
//                 std::cout << std::endl;
//             }
//             // battle = false;

//             Player_1_bits = 0;
//             Player_2_bits = 0;
//             for (int i = (int)(boardPP.size() - 1); i >= 0; i--) {
//                 for(int j = (int)(boardPP[i].size() - 1); j >= 0; j--) {

//                     if (boardPP[i][j] == 1)
//                         Player_1_bits = (Player_1_bits << 1) | 1;
//                     else
//                         Player_1_bits = (Player_1_bits << 1);

//                     if (boardPP[i][j] == 2)
//                         Player_2_bits = (Player_2_bits << 1) | 1;
//                     else
//                         Player_2_bits = (Player_2_bits << 1);
//                 }
//             }
//             std::cout << "Player_1_bits = " << Player_1_bits << std::endl;
//             std::cout << "Player_2_bits = " << Player_2_bits << std::endl;



//             AI test_player_1(boardPP, 1, 2);
//             begin = std::chrono::steady_clock::now();
//             auto rsl1 = test_player_1.bestMove(Player_2_bits, Player_1_bits);
//             end = std::chrono::steady_clock::now();

//             std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
//             std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
//             std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

//             std::cout << "Player 1" << " y = " << rsl1.first << ", z = " << rsl1.second << std::endl;
//             boardPP[rsl1.first][rsl1.second] = 1;
//             battle = !WinChecker(boardPP, 1).result();

//             for (int x = 0; x < 15; x++) {
//                 for (int y = 0; y < 15; y++)
//                     std::cout << (int) boardPP[x][y] << " ";
//                 std::cout << std::endl;
//             }
//             // // battle = false;
//         }



//         int i = 0;

//         //return WinChecker(boardPP, pieceNumber).result();
//         return 0;
//     }
// }

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