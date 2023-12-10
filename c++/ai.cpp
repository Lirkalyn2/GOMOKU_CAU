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
    std::list<ThreadPool>::iterator thread_it;
    GameState *data;

    for(int i = 0; i < MAIN_THREADS_NUMBER; i++) {
        main_square_thread_pool.push_back(ThreadPool());
        thread_it = main_square_thread_pool.end();
        thread_it--;
        thread_it->game_calculator_thread = std::thread(&AI::alphabeta_thread, this, thread_it);
        thread_it->stop = new std::mutex();
        thread_it->game_calculator_thread.detach();
    }

    for(int i = 0; i < LEAVES_THREADS_NUMBER; i++) {
        leaves_thread_pool.push_back(ThreadPool());
        thread_it = leaves_thread_pool.end();
        thread_it--;
        thread_it->game_calculator_thread = std::thread(&AI::leaves_thread, this, thread_it);
        thread_it->stop = new std::mutex();
        thread_it->stop->lock();
        thread_it->stop->unlock();
        thread_it->game_calculator_thread.detach();
    }

    for(int i = 0; i < MAX_MAIN_THREAD_MEM; i++) {
        data = nullptr;
        data = new GameState;
        main_square_memory_space.put(data);
    }

    for(int i = 0; i < MAX_LEAVES_MEM; i++) {
        data = nullptr;
        data = new GameState;
        leaves_memory_space.put(data);
    }

    main_square_memory_space.reset();
    leaves_memory_space.reset();
}

AI::~AI()
{
    GameState *data;

    // also delete thread's mutex

    main_square_memory_space.act_full();
    leaves_memory_space.act_full();

    while (!main_square_memory_space.empty())
    {
        data = main_square_memory_space.get().value();
        delete data;
    }
    while (!leaves_memory_space.empty())
    {
        data = leaves_memory_space.get().value();
        delete data;
    }
}

std::pair<int, int> AI::bestMove(uint256_t humanBits, uint256_t cpuBits)
{
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(_board);
    std::pair<int, int> move;
    int alpha = NINFINITY;
    int beta = INFINITY;
    int bestScore = NINFINITY;
    GameState *tmp = new GameState;
    std::vector<std::pair<std::pair<int, int>, int>> moves(squares.size(), std::make_pair(std::make_pair(0, 0), 0)); // inner most pair is for y and x, last one is the score.

    for (size_t i = 0; i < squares.size(); i++) {
        std::cout << "y = " << squares[i].first << ", x = " << squares[i].second << std::endl;
    }

    for (size_t i = 0; i < squares.size(); i++) {
        int y = squares[i].first;
        int x = squares[i].second;
        moves[i].first.first = y;
        moves[i].first.second = x;
        uint256_t pos = uint256_t((y * 15) + x);

        _board[y][x] = -1;
        cpuBits |= 1 << pos;

        tmp->matrix = _board;
        tmp->depth = 1;
        tmp->alpha = alpha;
        tmp->beta = beta;
        tmp->isAiTurn = false;
        tmp->playerBits = humanBits;
        tmp->opponentBits = cpuBits;
        tmp->move = (moves.begin() + i);
        main_square_flag.lock_shared();
        while (main_square_memory_space.full()); // wait for space to liberate.
        main_square_memory_space.swap_head_value(tmp);


        _board[y][x] = 0;
        cpuBits &= ~(1 << pos);
    }

    main_square_flag.lock(); // for the thread to finish their work.

    for (size_t i = 0; i < moves.size(); i++) {
        if (moves[i].second == 9999) {
            return std::make_pair(moves[i].first.first, moves[i].first.second);
        }

        if (moves[i].second > bestScore) {
            alpha = moves[i].second;
            bestScore = moves[i].second;
            move = std::make_pair(moves[i].first.first, moves[i].first.second);
        }
    }

    // for (size_t i = 0; i < squares.size(); i++) {
    //     // init thred mem here.
    //     // first change bord with y, x then copy it for the mutex. same for cpuBits.
    //     // each thread needs it's own _board, alpha, beta, humanBits, cpuBits // put all in a std::list
    //     // each square is a point on the queu, each point has locked the same shared mutex once and when the thread is done it free the shared mutex once.
    //     // main thread it try lock this shared mutex, so it as to wait for all the shared to be done.


    //     /*
    //         put each of bestMove's squares in a separate thread. (first thread pool)
    //         when a thread reaches the penultimate layer (right before max depth is reached), each of it's square should be put in a list to be multithreaded. (second thread pool)
    //         when all the leaves, have been computed the main square thread can continue it's works until done.

    //         for synchronisation, use shared mutex.
    //         lock mains square's shared mutex x times (x being number of leaves) (use shared lock here).
    //         put the leaves in the right  place to be computed
    //         ask to lock the shared mutex and wait for the mutex to be available (use NOT shared lock here)
    //         // end of main square's work

    //         now on the leaf side
    //         when leaf is computed
    //         store result
    //         unlock shared mutex once. (use shared unlock here)
    //         take new leaf
    //         // end of leaf's work

    //         // maybe separate the leaf in different smaller circular buffer to avoid to much fight on the mutex.

    //         // bonus, when a main thread is waiting for it's leaves, it's square can go in a waiiting pool and the thread takes anew square.
    //     */
    delete tmp;
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

    int best = isAiTurn ? -9999 : 9999;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(matrix);

    if ((depth + 1) >= MAX_DEPTH) {
        GameState *tmp = new GameState;
        std::shared_mutex yolo_leaves_flag;
        std::vector<std::pair<std::pair<int, int>, int>> moves(squares.size(), std::make_pair(std::make_pair(0, 0), 0)); // inner most pair is for y and x, last one is the score.


        for (size_t i = 0; i < squares.size(); i++) {
            int y = squares[i].first;
            int x = squares[i].second;
            uint256_t pos = ((y * 15) + x);

            matrix[y][x] = (isAiTurn ? -1 : 1);
            playerBits |= 1 << pos;

            // playerBits, depth, isAiTurn, opponentBits


            tmp->depth = (depth + 1); // may need a deep copy here.
            // tmp->alpha = alpha; // may need a deep copy here.
            // tmp->beta = beta; // may need a deep copy here.
            tmp->isAiTurn = !isAiTurn; // may need a deep copy here.
            tmp->playerBits = playerBits; // may need a deep copy here.
            tmp->opponentBits = opponentBits; // may need a deep copy here.
            tmp->move = (moves.begin() + i); // may need a deep copy here.
            // main_square_flag.lock_shared();
            yolo_leaves_flag.lock_shared();
            tmp->yolo_leaves_flag = &yolo_leaves_flag;
            while (leaves_memory_space.full()); // wait for space to liberate.
            leaves_memory_space.swap_head_value(tmp);

            // put in leaves mem

            matrix[y][x] = 0;
            playerBits &= ~(1 << pos);
        }
        delete tmp;
        yolo_leaves_flag.lock();
        for (size_t i = 0; i < moves.size(); i++) {
            // moves[i].second;
            if (isAiTurn) {
                if (moves[i].second >= beta) {
                    return moves[i].second;
                }

                best = std::max(moves[i].second, best);
                alpha = std::max(alpha, moves[i].second);
            }
            else {
                if (moves[i].second <= alpha) {
                    return moves[i].second;
                }

                best = std::min(moves[i].second, best);
                beta = std::min(beta, moves[i].second);
            }

            if (moves[i].second == 9999 && isAiTurn) {
                return moves[i].second;
            }
            else if (moves[i].second == -9999 && !isAiTurn) {
                return moves[i].second;
            }
        }
    }
    else {
        for (size_t i = 0; i < squares.size(); i++) {
            int y = squares[i].first;
            int x = squares[i].second;
            uint256_t pos = ((y * 15) + x);

            matrix[y][x] = (isAiTurn ? -1 : 1);
            playerBits |= 1 << pos;

            // int score = 0;

            // if ((depth + 1) >= MAX_DEPTH) {
            //     // put data in leaves
            // }
            // else
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
        const std::lock_guard<std::mutex> lock(*thread_it->stop);

        if (!main_square_memory_space.empty() && main_square_memory_space.swap_tail_value(data)) {

            data->move->second = alphabeta(data->matrix, data->depth, data->alpha, data->beta, data->isAiTurn, data->playerBits, data->opponentBits);
            main_square_flag.unlock_shared();
            std::cout << "unlock" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2)); // need shorter sleep
        // std::this_thread::sleep_for(std::chrono::duration<float>(1)); // need shorter sleep
    }
    delete data;
    return 0;
}

int AI::leaves_thread(std::list<ThreadPool>::iterator thread_it)
{
    GameState *data = new GameState();

    while (thread_it->is_alive) {
        const std::lock_guard<std::mutex> lock(*thread_it->stop);

        if (!leaves_memory_space.empty() && leaves_memory_space.swap_tail_value(data)) {

            if (checkWinner(data->playerBits, data->depth)) {
                data->move->second = data->isAiTurn ? 9999 : -9999;
            }
            else {
                size_t eval = staticEval(data->playerBits) - staticEval(data->opponentBits);
                data->move->second = data->isAiTurn ? eval : -eval;
            }
            data->yolo_leaves_flag->unlock_shared();

            // data->move->second = alphabeta(data->matrix, data->depth, data->alpha, data->beta, data->isAiTurn, data->playerBits, data->opponentBits);
            // data->move->second = alphabeta(data->matrix, data->depth, data->alpha, data->beta, data->isAiTurn, data->playerBits, data->opponentBits);
            // main_square_flag.unlock_shared();
            std::cout << "unlock leaves" << std::endl;
        }
        // maybe sleep here;
        std::this_thread::sleep_for(std::chrono::milliseconds(2)); // need shorter sleep
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
        std::cout << "end = " << ((rsl.first * 100) + rsl.second) << std::endl;


        return ((rsl.first * 100) + rsl.second);
        // return std::to_string(rsl.first) + " " + std::to_string(rsl.second);

        // return std::string("ok").c_str();

        // return WinChecker(boardPP, pieceNumber).result();
        // return std::string();
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