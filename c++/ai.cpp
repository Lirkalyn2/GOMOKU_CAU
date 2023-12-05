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
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(_board); // sometimes have repetisions of squares.
    std::pair<int, int> move;
    int alpha = NINFINITY;//-std::numeric_limits<int>::infinity();
    int beta = INFINITY;//std::numeric_limits<int>::infinity();
    int bestScore = NINFINITY;//-std::numeric_limits<int>::infinity();

    // for (size_t i = 0; i < squares.size(); i++) {
    //     // int y = squares[i].first;
    //     // int x = squares[i].second;
    //     int x = squares[i].first;
    //     int y = squares[i].second;
    //     // uint256_t pos = uint256_t((y * 15) + x);
    //     uint256_t pos = uint256_t((x * 15) + y);

    //     _board[x][y] = -1;
    //     cpuBits |= 1 << pos;
    //     // int score = 0;
    //     int score = alphabeta(_board, 1, alpha, beta, false, humanBits, cpuBits);

    //     _board[x][y] = 0;
    //     cpuBits &= ~(1 << pos);

    //     // console.log('%s evaluated to %s', JSON.stringify([y, x]), score);

    //     // if we find a win, play it immediately
    //     if(score == 9999){
    //         return std::make_pair(x, y);
    //     }

    //     if(score > bestScore){
    //         alpha = score;
    //         bestScore = score;
    //         move = std::make_pair(x, y);
    //     }
    // }

    for (size_t i = 0; i < squares.size(); i++) {
        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = uint256_t((y * 15) + x);

        _board[y][x] = -1;
        cpuBits |= 1 << pos;
        // int score = 0;
        int score = alphabeta(_board, 1, alpha, beta, false, humanBits, cpuBits);

        _board[y][x] = 0;
        cpuBits &= ~(1 << pos);

        // console.log('%s evaluated to %s', JSON.stringify([y, x]), score);

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

    for(size_t i = 0; i < my_board.size(); i++) {
        for(size_t j = 0; j < my_board[i].size(); j++) {
            if(my_board[i][j] != 0) {
                addAdjacent(i, j, tmp_rsl, my_board);
            }
        }
    }

    // std::sort(tmp_rsl.begin(), tmp_rsl.end(), [](const auto& x, const auto& y) {
    //     return x.first < y.first;
    // });

    std::sort(tmp_rsl.begin(), tmp_rsl.end());

    // std::transform(tmp_rsl.begin(), tmp_rsl.end(), tmp_rsl.begin(), [](const auto& z) {
    //     rsl.push_back(std::make_pair(z.first >> 4, z.first & 0x0F));
    // });

    for (size_t i = 0; i < tmp_rsl.size(); i++) {
        // std::cout << "tmp_rsl[" << i << "] = " << (int)tmp_rsl[i] << std::endl;
        // rsl.push_back(std::make_pair((tmp_rsl[i] & 0x0f), (tmp_rsl[i] >> 4)));
        rsl.push_back(std::make_pair((tmp_rsl[i] >> 4), (tmp_rsl[i] & 0x0f)));
    }
    return rsl;
}

void AI::addAdjacent(const char x, const char y, std::vector<char> &list, const std::vector<std::vector<char>> &my_board) // can be multi threaded
{
    // put((y + 1), x, list, my_board);
    // put((y - 1), x, list, my_board);
    // put(y, (x + 1), list, my_board);
    // put(y, (x - 1), list, my_board);
    // put((y + 1), (x + 1), list, my_board);
    // put((y - 1), (x + 1), list, my_board);
    // put((y - 1), (x - 1), list, my_board);
    // put((y + 1), (x - 1), list, my_board);
    put(x, (y + 1), list, my_board);
    put(x, (y - 1), list, my_board);
    put((x + 1), y, list, my_board);
    put((x - 1), y, list, my_board);
    put((x + 1), (y + 1), list, my_board);
    put((x + 1), (y - 1), list, my_board);
    put((x - 1), (y - 1), list, my_board);
    put((x - 1), (y + 1), list, my_board);
}

void AI::put(const char y, const char x, std::vector<char> &list, const std::vector<std::vector<char>> &my_board)
{
    // if (!(x < 0 || y < 0 || x > (char)(my_board.size() - 2) || y > (char)(my_board.size() - 2) || !(my_board[y][x] == 0)))
    // if ((x >= 0 && x <= (char)(my_board.size() - 2)) && (y >= 0 && y <= (char)(my_board.size() - 2)) && (my_board[x][y] == 0))
    //     list.push_back(((x << 4) | y));

    if ((x >= 0 && x <= (char)(my_board.size() - 2)) && (y >= 0 && y <= (char)(my_board.size() - 2)) && (my_board[y][x] == 0))
        list.push_back(((y << 4) | x));
}



int AI::alphabeta(std::vector<std::vector<char>> matrix, int depth, int alpha, int beta, bool isAiTurn, uint256_t playerBits, uint256_t opponentBits)
{
    // totalCalcs++;
    if (checkWinner(opponentBits, depth)) { // WinChecker(board, (isAiTurn ? _AI_Color : _Enemy_Color)).result();
        return isAiTurn ? -9999 : 9999;
    }

    // stop at MAX_DEPTH
    if (depth >= MAX_DEPTH){
        if (checkWinner(playerBits, depth)) {
            return isAiTurn ? 9999 : -9999;
        }
        else {
            size_t eval = staticEval(playerBits) - staticEval(opponentBits);
            return isAiTurn ? eval : -eval;
        }
    }

    int best = isAiTurn ? -9999 : 9999;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(matrix);//, depth);

    for(size_t i = 0; i < squares.size(); i++){
        // if(i === 0){
        //     if(depth === 1){
        //         console.log()
        //     } else if(depth === 2){
        //         console.log()
        //     } else if(depth === 3){
        //         console.log()
        //     } else if(depth === 4){
        //         console.log()
        //     }
        // }

        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = ((y * 15) + x);

        matrix[y][x] = (isAiTurn ? -1 : 1);
        playerBits |= 1 << pos;

        int score = alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);

        matrix[y][x] = 0;
        playerBits &= ~(1 << pos);

        if(isAiTurn){
            if(score >= beta){
                return score;
            }

            best = std::max(score, best);
            alpha = std::max(alpha, score);
        }
        else {
            if(score <= alpha){
                return score;
            }

            best = std::min(score, best);
            beta = std::min(beta, score);
        }

        if(score == 9999 && isAiTurn){
            return score;
        }
        else if(score == -9999 && !isAiTurn){
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
        if((v & matrix) == v) return false;
        v *= 2;
    }

    return 0;
}

bool AI::matchBitmask(const uint256_t &matrix, uint256_t &mask)
{
    for(size_t i = 0; mask <= matrix; i++, mask *= 2){
        if((i % 15) > 10) continue;
        if((mask & matrix) == mask) return true;
    }

    return false;
}



size_t AI::staticEval(const uint256_t &matrix)
{
    // if(checkCache('evals', matrix)){
    //     return checkCache('evals', matrix);
    // }

    size_t total = 0;
    uint256_t hMask = 3;
    uint256_t vMask = 32769;
    uint256_t d1Mask = 65537;
    uint256_t d2Mask = 16385;

    total += matchMask(hMask, matrix);
    total += matchMask(d1Mask, matrix);
    total += matchMask(d2Mask, matrix);

    while(vMask <= matrix){
        if((vMask & matrix) == vMask) {
            total++;
        }
        vMask *= 2;
    }

    //putCache('evals', matrix, total);

    return total;
}

size_t AI::matchMask(uint256_t &mask, const uint256_t &matrix)
{
    size_t count = 0;

    for(size_t i = 0; mask <= matrix; i++, mask *= 2){
        if((i%15) > 10) continue;
        if((mask & matrix) == mask) count++;
    }

    return count;
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
        uint256_t Player_1_bits = 0;
        uint256_t Player_2_bits = 0;

        while (battle) {

            Player_1_bits = 0;
            Player_2_bits = 0;
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
            std::cout << "Player_1_bits = " << Player_1_bits << std::endl;
            std::cout << "Player_2_bits = " << Player_2_bits << std::endl;



            AI test_player_2(boardPP, 2, 1);
            begin = std::chrono::steady_clock::now();
            auto rsl2 = test_player_2.bestMove(Player_1_bits, Player_2_bits);
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

            Player_1_bits = 0;
            Player_2_bits = 0;
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
            std::cout << "Player_1_bits = " << Player_1_bits << std::endl;
            std::cout << "Player_2_bits = " << Player_2_bits << std::endl;



            AI test_player_1(boardPP, 1, 2);
            begin = std::chrono::steady_clock::now();
            auto rsl1 = test_player_1.bestMove(Player_2_bits, Player_1_bits);
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