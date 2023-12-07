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

    // std::list<ThreadPool>::iterator thread_it;

    // for(int i = 0; i < THREAD_NUMBER; i++) {
    //     thread_pool.push_back(ThreadPool());
    //     thread_it = thread_pool.end();
    //     thread_it--;
    //     thread_it->parity_calculator_thread = std::thread(&ParityMaker::ParityCalculater, this, thread_it, i);
    //     thread_it->parity_calculator_thread.detach();
    // }
    //     // allocate thread_memory_space
    // use new on std::list and try to do it 
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

    // for (size_t i = 0; i < squares.size(); i++) {
    //     std::cout << "y = " << squares[i].first << ", x = " << squares[i].second << std::endl;
    // }

    for (size_t i = 0; i < squares.size(); i++) {
        // init thred mem here.
        // first change bord with y, x then copy it for the mutex. same for cpuBits.
        // each thread needs it's own _board, alpha, beta, humanBits, cpuBits // put all in a std::list
        // each square is a point on the queu, each point has locked the same shared mutex once and when the thread is done it free the shared mutex once.
        // main thread it try lock this shared mutex, so it as to wait for all the shared to be done.


        // /*
        // we need to queu/list. one for the imediat work and one the the work who is waiting for more work to be done.

        // Imagine you're a thrad, you did some calc and you need more paralle work to be done.
        // you register the aditionnal work in the working list and move your current work in the waiitng list.
        // when the working list is empty, the threads check the work in the waiting list.
        // Do it until completed.
        // We will need a LOT  of pointers or iterator on list. // check the work done on babel whit the swap on the circular buffer.
        // // remmeber to initialise as much memory on the heap (new keyword) as possible.
        // */


        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = uint256_t((y * 15) + x);

        _board[y][x] = -1;
        // std::cout << "1 " << y << ", " << x << " = " << cpuBits << "!!!" << std::endl;
        cpuBits |= 1 << pos;
        int score = alphabeta(_board, 1, alpha, beta, false, humanBits, cpuBits);

        // std::cout << "Final score for X:" << x << " et Y:" << y << " score:" << score << " Fore evaluation : " << staticEval(humanBits) << " evel" << staticEval(cpuBits) << std::endl;

        _board[y][x] = 0;
        cpuBits &= ~(1 << pos); // check if cpuBits changes!!! // always stays the same
        // std::cout << "2 " << y << ", " << x << " = " << cpuBits << "!!!" << std::endl;

        // end of thread.

        // if we find a win, play it immediately
        if (score == 9999) {
            return std::make_pair(y, x);
        }

        if (score > bestScore) {
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
    std::vector<unsigned char> tmp_rsl;

    for (size_t i = 0; i < my_board.size(); i++) {
        for (size_t j = 0; j < my_board[i].size(); j++) {
            if (my_board[i][j] != 0) {
                addAdjacent(i, j, tmp_rsl, my_board);
            }
        }
    }

    // std::sort(tmp_rsl.begin(), tmp_rsl.end(), [](const auto& x, const auto& y) {
    //     return x.first < y.first;
    // });

    // std::sort(tmp_rsl.begin(), tmp_rsl.end());

    // std::transform(tmp_rsl.begin(), tmp_rsl.end(), tmp_rsl.begin(), [](const auto& z) {
    //     rsl.push_back(std::make_pair(z.first >> 4, z.first & 0x0F));
    // });

    for (size_t i = 0; i < tmp_rsl.size(); i++) {
        // std::cout << "tmp_rsl[" << i << "] = " << (int)tmp_rsl[i] << std::endl;
        int y = (tmp_rsl[i] & 0x0f);
        int x = (tmp_rsl[i] >> 4);
        // std::cout << "y = " << y << ", x = " << x << std::endl;
        if (my_board[y][x] <= 0) // band aid
            rsl.push_back(std::make_pair(y, x));
            // rsl.push_back(std::make_pair((tmp_rsl[i] & 0x0f), (tmp_rsl[i] >> 4)));
        // rsl.push_back(std::make_pair((tmp_rsl[i] >> 4), (tmp_rsl[i] & 0x0f)));
    }
    return rsl;
}

void AI::addAdjacent(const char i, const char j, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board) // can be multi threaded
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

void AI::put(const char y, const char x, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board)
{
    unsigned char combination = ((y << 4) | x);

    for (size_t i = 0; i < list.size(); i++)
        if (list[i] == combination)
            return;

    if ((x >= 0 && x <= (unsigned char)(my_board.size() - 2)) && (y >= 0 && y <= (unsigned char)(my_board.size() - 2)) && (my_board[y][x] == 0)) // maybe change it <=
        list.push_back(combination);
}

int AI::alphabeta(std::vector<std::vector<char>> matrix, int depth, int alpha, int beta, bool isAiTurn, uint256_t playerBits, uint256_t opponentBits)
{
    // totalCalcs++;
    if (checkWinner(opponentBits, depth)) { // WinChecker(board, (isAiTurn ? _AI_Color : _Enemy_Color)).result();
//        std::cout << "Critical defeat! And a " << (isAiTurn ? " bad" : "good") << " one!!!" << std::endl;
        return isAiTurn ? -9999 : 9999;
    }

    // stop at MAX_DEPTH
    if (depth >= MAX_DEPTH) {
        if (checkWinner(playerBits, depth)) {
            return isAiTurn ? 9999 : -9999;
        }
        else {
            size_t eval = staticEval(playerBits) - staticEval(opponentBits);
            return isAiTurn ? eval : -eval;
        }
    }

    int best = isAiTurn ? -9999 : 9999;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(matrix);

    for (size_t i = 0; i < squares.size(); i++) {
        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = ((y * 15) + x);

        matrix[y][x] = (isAiTurn ? -1 : 1);
        playerBits |= 1 << pos;

        int score = alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);

        matrix[y][x] = 0;
        playerBits &= ~(1 << pos);

        // std::cout << "For X:" << x << " et Y:" << y << " score:" << score << " for " << (isAiTurn ? "us" : "human") << std::endl;
        if (isAiTurn) {
            if (score >= beta) {
                return score;
            }

            best = std::max(score, best);
            alpha = std::max(alpha, score);
        }
        else {
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

bool AI::checkWinner(uint256_t &bits, int &depth)
{
    // if(this.totalMoves + depth < 9){
    //     return false;
    // }

    // if(checkCache('winners', bits) !== false){
    //     return checkCache('winners', bits);
    // }

    if (hasWon(bits)) {
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
    uint256_t d1 = 281479271743489; // should be 18447025552981295105 but compiller said NO!!!
    d1 *= 65536;
    d1 += 1;
    uint256_t d2 = 1152991877646254096;

    if (matchBitmask(matrix, h)) return true;
    if (matchBitmask(matrix, d1)) return true;
    if (matchBitmask(matrix, d2)) return true;

    // vertical is a "bit" different :)
    while (v <= matrix) {
        if ((v & matrix) == v) return true;
        v *= 2;
    }

    return false;
}

bool AI::matchBitmask(const uint256_t &matrix, uint256_t &mask)
{
    for (size_t i = 0; mask <= matrix; i++, mask *= 2) {
        if ((i % 15) > 10) continue;
        if ((mask & matrix) == mask) return true;
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

    while (vMask <= matrix) {
        if ((vMask & matrix) == vMask) {
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

    for (size_t i = 0; mask <= matrix; i++, mask *= 2) {
        if ((i % 15) > 10) continue;
        if ((mask & matrix) == mask) count++;
    }

    return count;
}

// size_t AI::staticEval(const uint256_t &matrix)
// {

//     size_t total = 0;
//     uint256_t hMask = 3;
//     uint256_t vMask = 32769;
//     uint256_t d1Mask = 65537;
//     uint256_t d2Mask = 16385;

//     total += matchMask(hMask, matrix);
//     total += matchMask(d1Mask, matrix);
//     total += matchMask(d2Mask, matrix);

//     for (int i = 0; i < 15; i++, vMask *= 2) {
//         total += calcVertical(vMask, matrix);
//     }

//     return total;
// }

// size_t AI::matchMask(uint256_t &mask, const uint256_t &matrix)
// {
//     size_t count = 0;
//     int streak = 0;

//     for (size_t i = 0; mask <= matrix; i++, mask *= 2) {
//         if ((i%15) > 13) continue;
//         if ((mask & matrix) == mask) {
//             streak++;
//             count += calcStreak(streak);
//         }
//         else {
//             streak = 0;
//         }
//     }

//     return count;
// }

// size_t AI::calcVertical(uint256_t vMask, const uint256_t &matrix) {
//     int streak = 0;
//     int total = 0;

//     while (vMask <= matrix) {
//         if ((vMask & matrix) == vMask) {
//             streak++;
//             total += calcStreak(streak);
//         }
//         else {
//             streak = 0;
//         }
//         vMask *= 32768;
//     }
//     return total;
// }

// size_t AI::calcStreak(int streak) {
//     size_t pointArray[6] = {0, 2, 8, 64, 512, 9999};
//     return (pointArray[streak]);
// }

int AI::alphabeta_thread(std::list<ThreadPool>::iterator thread_it) // switch to void
{
    GameState *data = new GameState();

    while (thread_it->is_alive) {
        const std::lock_guard<std::mutex> lock(thread_it->stop);

        if (!game_memory_space_working.empty() && game_memory_space_working.swap_tail_value(data)) {
            // compute
            // region basic compute

            // calc best and get square

            // if (data->depth >= MAX_DEPTH) { // nop already done above
            if () {
                // compute alpha beta

            }
            else {
                // std::vector<GameState *>::iterator results_it;
                // GameState *new_data = new GameState();

                // for (size_t i = 0; i < squares.size(); i++) {
                //     int y = squares[i].first;
                //     int x = squares[i].second;
                //     uint256_t pos = ((y * 15) + x);

                //     data->matrix[y][x] = (data->isAiTurn ? -1 : 1);
                //     data->playerBits |= 1 << pos;

                //     // // //alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);
                //     new_data->matrix = data->matrix; // may need a deep copy here.
                //     new_datat->depth = (data->depth + 1);
                //     new_data->alpha = data->alpha;
                //     new_data->beta = data->beta;
                //     new_data->isAiTurn = !data->isAiTurn;
                //     new_data->opponentBits = data->opponentBits;
                //     new_data->playerBits = data->playerBits;
                //     new_data->gameStateProtector.lock_shared();// ??? should we lock this one or the one one above in the tree ???
                //     game_memory_space_working.swap_head_value(new_data);

                //     matrix[y][x] = 0;
                //     playerBits &= ~(1 << pos);
                // }
                // delete new_data; // ??? can we save it ???
            }

        }
        else if (!game_memory_space_waiting.empty() && game_memory_space_waiting.swap_tail_value(data)) {
            // compute
        }
    }
}

int AI::alphabeta_thread(std::list<ThreadPool>::iterator thread_it) // switch to void
{
    GameState *data = new GameState();

    while (thread_it->is_alive) {
        const std::lock_guard<std::mutex> lock(thread_it->stop);

        if (!game_memory_space_working.empty() && game_memory_space_working.swap_tail_value(data)) {
            // compute
            // region basic compute
            if (checkWinner(data->opponentBits, data->depth)) { // WinChecker(board, (isAiTurn ? _AI_Color : _Enemy_Color)).result();
        //        std::cout << "Critical defeat! And a " << (isAiTurn ? " bad" : "good") << " one!!!" << std::endl;
                data->score = data->isAiTurn ? -9999 : 9999;
                data->gameStateProtector.unlock_shared();
                // break from everything
            }

            // stop at MAX_DEPTH
            if (data->depth >= MAX_DEPTH) {
                if (checkWinner(data->playerBits, data->depth)) {
                    data->score = data->isAiTurn ? 9999 : -9999;
                    data->gameStateProtector.unlock_shared();
                    // break from everything
                }
                else {
                    size_t eval = staticEval(data->playerBits) - staticEval(data->opponentBits);
                    data->score = data->isAiTurn ? eval : -eval;
                    data->gameStateProtector.unlock_shared();
                    // break from everything
                }
            }
            // endregion basic compute. preferably

            int best = data->isAiTurn ? -9999 : 9999;
            std::vector<std::pair<int, int>> squares = getSquaresToCheck(data->matrix);
            // endregion basic compute.

            if (data->depth >= MAX_DEPTH) {
                // compute alpha beta

            }
            else {
                // std::vector<GameState *>::iterator results_it;
                GameState *new_data = new GameState();

                for (size_t i = 0; i < squares.size(); i++) {
                    int y = squares[i].first;
                    int x = squares[i].second;
                    uint256_t pos = ((y * 15) + x);

                    data->matrix[y][x] = (data->isAiTurn ? -1 : 1);
                    data->playerBits |= 1 << pos;

                    // // //alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);
                    new_data->matrix = data->matrix; // may need a deep copy here.
                    new_data->depth = (data->depth + 1);
                    new_data->alpha = data->alpha;
                    new_data->beta = data->beta;
                    new_data->isAiTurn = !data->isAiTurn;
                    new_data->opponentBits = data->opponentBits;
                    new_data->playerBits = data->playerBits;
                    new_data->gameStateProtector.lock_shared();// ??? should we lock this one or the one one above in the tree ???
                    game_memory_space_working.swap_head_value(new_data);

                    // // data->results.push_back(new GameState());
                    // // results_it = data->results.end();
                    // // results_it--;
                    // // //alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);
                    // results_it->matrix = matrix; // may need a deep copy here.
                    // results_it->depth = (depth + 1);
                    // results_it->alpha = alpha;
                    // results_it->beta = beta;
                    // results_it->isAiTurn = !isAiTurn;
                    // results_it->opponentBits = opponentBits;
                    // results_it->playerBits = playerBits;
                    // results_it->gameStateProtector.lock_shared();// ??? should we lock this one or the one one above in the tree ???
                    game_memory_space_working.swap_head_value(new_data);

    // std::list<ThreadPool>::iterator thread_it;

    // for(int i = 0; i < THREAD_NUMBER; i++) {
    //     thread_pool.push_back(ThreadPool());
    //     thread_it = thread_pool.end();
    //     thread_it--;
    //     thread_it->parity_calculator_thread = std::thread(&ParityMaker::ParityCalculater, this, thread_it, i);
    //     thread_it->parity_calculator_thread.detach();
    // }

                    data->matrix[y][x] = 0;
                    data->playerBits &= ~(1 << pos);
                }
                delete new_data; // ??? can we save it ???
            }

            // for (size_t i = 0; i < squares.size(); i++) {
            //     // int y = squares[i].first;
            //     // int x = squares[i].second;
            //     // uint256_t pos = ((y * 15) + x);

            //     // matrix[y][x] = (isAiTurn ? -1 : 1);
            //     // playerBits |= 1 << pos;

            //     // int score = alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);

            //     // matrix[y][x] = 0;
            //     // playerBits &= ~(1 << pos);

            //     // std::cout << "For X:" << x << " et Y:" << y << " score:" << score << " for " << (isAiTurn ? "us" : "human") << std::endl;
            //     if (isAiTurn) {
            //         if (score >= beta) {
            //             return score;
            //         }

            //         best = std::max(score, best);
            //         alpha = std::max(alpha, score);
            //     }
            //     else {
            //         if (score <= alpha) {
            //             return score;
            //         }

            //         best = std::min(score, best);
            //         beta = std::min(beta, score);
            //     }

            //     if (score == 9999 && isAiTurn) {
            //         return score;
            //     }
            //     else if (score == -9999 && !isAiTurn) {
            //         return score;
            //     }
            // }

        }
        else if (!game_memory_space_waiting.empty() && game_memory_space_waiting.swap_tail_value(data)) {
            // compute
        }
    }
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
            for (int j = (int)(boardPP[i].size() - 1); j >= 0; j--) {

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

    // data[0][0] = 1;
    data[10][1] = 1;

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