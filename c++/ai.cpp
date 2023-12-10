/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** ai
*/

#include "ai.hpp"
#include <chrono>
#include <cmath>

uint256_t BitsSetTable256[256];

// Function to initialise the lookup table
void AI::initialize()
{

    // To initially generate the
    // table algorithmically
    BitsSetTable256[0] = 0;
    for (int i = 0; i < 256; i++) {
        BitsSetTable256[i] = (i & 1) +
        BitsSetTable256[i / 2];
    }
}

// Function to return the count
// of set bits in n.
// Original idea: https://www.geeksforgeeks.org/count-set-bits-in-an-integer/  3. Using lookup table
uint AI::countSetBits(unsigned char n)
{
    return (BitsSetTable256[n]);
}

AI::AI(std::vector<std::vector<char>> board, char Ai_color, char Enemy_Color)
{
    _board = board;
    _AI_Color = Ai_color;
    _Enemy_Color = Enemy_Color;
    initialize();
    std::list<ThreadPool>::iterator thread_it;
    GameState *data;

    for(int i = 0; i < MAIN_THREADS_NUMBER; i++) {
        main_square_thread_pool.push_back(ThreadPool());
        thread_it = main_square_thread_pool.end();
        thread_it--;
        thread_it->game_calculator_thread = std::thread(&AI::alphabeta_thread, this, thread_it);
        thread_it->game_calculator_thread.detach();
    }

    for(int i = 0; i < LEAVES_THREADS_NUMBER; i++) {
        leaves_thread_pool.push_back(ThreadPool());
        thread_it = leaves_thread_pool.end();
        thread_it--;
        thread_it->game_calculator_thread = std::thread(&AI::leaves_thread, this, thread_it/*, the main thread personnalshared mutex*/);
        thread_it->game_calculator_thread.detach();
    }

    for(int i = 0; i < MAX_MAIN_THREAD_MEM; i++) {
        data = nullptr;
        data = new GameState;
        // data->matrix = new std::vector<std::vector<char>>();
        main_square_memory_space.put(data);
        // main_square_memory_space.put((*data));
    }

    for(int i = 0; i < MAX_LEAVES_MEM; i++) {
        data = nullptr;
        data = new GameState;
        // data->matrix = new std::vector<std::vector<char>>();
        leaves_memory_space.put(data);
        // leaves_memory_space.put((*data));
    }

    main_square_memory_space.reset();
    leaves_memory_space.reset();
    //     // allocate thread_memory_space
    // use new on std::list and try to do it
}

AI::~AI()
{
    GameState *data;

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
    // std::list<std::pair<std::pair<int, int>, int>> moves; // inner most pair is for y and x, last one is the score.
    std::vector<std::pair<std::pair<int, int>, int>> moves(squares.size(), std::make_pair(std::make_pair(0, 0), 0)); // inner most pair is for y and x, last one is the score.
    // std::list<int> scores;

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

        tmp->matrix = _board; // may need a deep copy here.
        // for (size_t j = 0; j < _board.size(); j++)
        //     for (size_t k = 0; k < _board[j].size(); k++)
        //         tmp->matrix[j][k] = _board[j][k];

        // std::cout << "alpha = " << static_cast<void *>(&((*tmp).matrix[0][0])) << std::endl;
        std::cout << "matrix_alpha = " << static_cast<void *>(&tmp->matrix) << std::endl;
        // std::cout << "alpha = " << static_cast<void *>(&(&tmp.matrix[0])) << std::endl;
        std::cout << "board_alpha = " << static_cast<void *>(&_board) << std::endl;
        std::cout << "board_alpha[0][0] = " << static_cast<void *>(&_board[0][0]) << std::endl;
        std::cout << std::endl;



        tmp->depth = 1; // may need a deep copy here.
        tmp->alpha = alpha; // may need a deep copy here.
        tmp->beta = beta; // may need a deep copy here.
        tmp->isAiTurn = false; // may need a deep copy here.
        tmp->playerBits = humanBits; // may need a deep copy here.
        tmp->opponentBits = cpuBits; // may need a deep copy here.
        tmp->move = (moves.begin() + i); // may need a deep copy here.
        main_square_flag.lock_shared();
        while (main_square_memory_space.full()); // wait for space to liberate.
        main_square_memory_space.swap_head_value(tmp);


        _board[y][x] = 0;
        cpuBits &= ~(1 << pos);
    }

    main_square_flag.lock(); // for the thread to finish their work.

    for (size_t i = 0; i < moves.size(); i++) {
        // moves[1];
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


    //     int y = squares[i].first;
    //     int x = squares[i].second;
    //     uint256_t pos = uint256_t((y * 15) + x);

    //     _board[y][x] = -1;
    //     // std::cout << "1 " << y << ", " << x << " = " << cpuBits << "!!!" << std::endl;
    //     cpuBits |= 1 << pos;
    //     int score = alphabeta(_board, 1, alpha, beta, false, humanBits, cpuBits);

    //     // std::cout << "Final score for X:" << x << " et Y:" << y << " score:" << score << " Fore evaluation : " << staticEval(humanBits) << " evel" << staticEval(cpuBits) << std::endl;

    //     _board[y][x] = 0;
    //     cpuBits &= ~(1 << pos); // check if cpuBits changes!!! // always stays the same
    //     // std::cout << "2 " << y << ", " << x << " = " << cpuBits << "!!!" << std::endl;

    //     // end of thread.

    //     // if we find a win, play it immediately
    //     if (score == 9999) {
    //         return std::make_pair(y, x);
    //     }

    //     if (score > bestScore) {
    //         alpha = score;
    //         bestScore = score;
    //         move = std::make_pair(y, x);
    //     }
    // }
    return move;
}

std::vector<std::pair<int, int>> AI::getSquaresToCheck(const std::vector<std::vector<char>> &my_board)
{
    std::vector<std::pair<int, int>> rsl;
    std::vector<unsigned char> tmp_rsl;

    for (size_t y = 0; y < my_board.size(); y++) {
        for (size_t x = 0; x < my_board[y].size(); x++) {
            if (my_board[y][x] != 0) {
                addAdjacent(y, x, tmp_rsl, my_board);
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
        int x = (tmp_rsl[i] & 0x0f);
        int y = (tmp_rsl[i] >> 4);
        std::cout << "Adding Y:" << y << " X:" << x << std::endl;
        if (my_board[y][x] <= 0) { // band aid
        std::cout << "Yes!" << std::endl;
            rsl.push_back(std::make_pair(y, x));
        }
    }
    return rsl;
}

void AI::addAdjacent(const char y, const char x, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board) // can be multi threaded
{
    put((y + 1), x, list, my_board);
    put((y - 1), x, list, my_board);
    put(y, (x + 1), list, my_board);
    put(y, (x - 1), list, my_board);
    put((y + 1), (x + 1), list, my_board);
    put((y - 1), (x + 1), list, my_board);
    put((y - 1), (x - 1), list, my_board);
    put((y + 1), (x - 1), list, my_board);
}

void AI::put(const char y, const char x, std::vector<unsigned char> &list, const std::vector<std::vector<char>> &my_board)
{
    unsigned char combination = ((y << 4) | x);

    for (size_t i = 0; i < list.size(); i++)
        if (list[i] == combination)
            return;

//    std::cout << "For X:" << (int) x << " Y:" << (int) y << " " << (bool) (x >= 0 && x <= (char)(my_board.size() - 2)) << " " << (bool) (y >= 0 && y <= (char)(my_board.size() - 2)) << " " << (bool) (my_board[y][x] == 0) << std::endl;
    if ((x >= 0 && x <= (char)(my_board.size() - 2)) && (y >= 0 && y <= (char)(my_board.size() - 2)) && (my_board[y][x] == 0))
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
        } else {
//            size_t eval = staticEval(playerBits) - staticEval(opponentBits);
            size_t eval = staticREval(playerBits, opponentBits) - staticREval(opponentBits, playerBits);
            std::cout << "Comparative. Original:" /* << staticEval(playerBits) - staticEval(opponentBits)*/ << " and New:" << eval << std::endl;
            return isAiTurn ? eval : -eval;
        }
    }

    int best = isAiTurn ? -9999 : 9999;
    std::vector<std::pair<int, int>> squares = getSquaresToCheck(matrix);

    // if (depth + 1) >= MAX_DEPTH put squares in mem pool.
    // else for loop

    for (size_t i = 0; i < squares.size(); i++) {
        int y = squares[i].first;
        int x = squares[i].second;
        uint256_t pos = ((y * 15) + x);

        matrix[y][x] = (isAiTurn ? -1 : 1);
        playerBits |= 1 << pos;

//        std::cout << "For X:" << x << " et Y:" << y << std::endl;
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

// Prints n bits of this sequence in order.
void printSequence(uint256_t sequence, int n, int endl) {
    for (int i = n - 1, j = 0; i >= 0; i--, j++) {
        bool bit = (sequence >> i) & 1;
        std::cout << bit;
        if ((j + 1) % endl == 0) {
            std::cout << std::endl;
        }
    }
}

// Prendre les 5 bits uniquement et les mettre dans un char pour countSetBits()
char extractSequence(uint256_t mask, int distance, int i) {
    char sequence = 0;

//    if (i == -1)
//        printSequence(mask, 225, 15);
    for (int i = 0; i < 5; i++) {
        sequence = sequence | static_cast<bool>(mask & 1);
        mask = mask >> distance;
        sequence = sequence << 1;
    }
//    std::cout << "extractSequence yelds:" << (int) sequence << "and the distance is " << distance << std::endl;
    return (sequence);
}

// En gros, on veut passer sur chaque case, et
size_t AI::staticREval(const uint256_t &matrix, const uint256_t &opponentMatrix) {
//    std::cout << "Enter REval, playerSequence is:" << std::endl;
//    printSequence(matrix, 15 * 15, 15);
    size_t total = 0;
    uint256_t h = 31;
    uint256_t v = 1152956690052710401;
    // uint256_t d1 = 18447025552981295105;
    uint256_t d1 = 281479271743489;
    d1 *= 65536;
    d1 += 1;
    uint256_t d2 = 1152991877646254096;

    uint256_t mask = 0;
    uint256_t tmp = 0;
    unsigned char sequence = 0;

    // 15 * 15 - 5: limite théorique de ce qu'on va évaluer, à savoir le dernier point d'où 5 cases avec horizontales sont possibles.
    // Au stratup, cela prendrait trop de temps pour chercher à travers les valeurs d'un uint256_t dans un array qui de toutes façons serait trop grand,
    // Donc on doit extraire à la mano les 5 bits et ceux-ci uniquement et comparer ces derniers uniquement dans l'array.
    for (size_t i = 0; i <= 220; i++, mask = mask << 1) {
        tmp = matrix >> mask;
//        std::cout << "I:" << i << " ";
//        std::cout << (i % 15 <= 10) << "   ";
//        std::cout << (i <= 165) << " ";
//        std::cout << (((opponentMatrix >> mask) & v) == 0);// << std::endl;

        if (i % 15 >= 4 && i <= 165 && ((opponentMatrix >> mask) & d2) == 0) { // Diagonale arrière
//            std::cout << "dia arr" << std::endl;
            sequence = extractSequence(tmp, 14, i);
            total += calcStreak(countSetBits(sequence));
        }
        if (i % 15 <= 10 && i <= 161 && ((opponentMatrix >> mask) & d1) == 0) { // Diagonale avant
//            std::cout << "dia av" << std::endl;
            sequence = extractSequence(tmp, 16, i);
            total += calcStreak(countSetBits(sequence));
        }
        if (i % 15 <= 10 && ((opponentMatrix >> mask) & h) == 0) { // Horizontale
//            std::cout << "hor" << std::endl;
            sequence = extractSequence(tmp, 1, i);
            total += calcStreak(countSetBits(sequence));
        }
        if (i <= 165 && ((opponentMatrix >> mask) & v) == 0) { // Verticale
            sequence = extractSequence(tmp, 15, (i == 0) ? -1 : i);
            total += calcStreak(countSetBits(sequence));
//            std::cout << "ver:" << (int) sequence << ", Bits:" << countSetBits(sequence) << ", Tot: " << total << std::endl;
        }
        if (mask == 0) {
            mask = 1;
        }
    }
//    std::cout << "Pong:" << total << std::endl;
//    std::cout << "Exit REval" << std::endl;
    return (total);
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
    thread_it->stop = new std::mutex(); // maybe put that in its initialiser

    while (thread_it->is_alive) {
        const std::lock_guard<std::mutex> lock(*thread_it->stop);

        if (!main_square_memory_space.empty() && main_square_memory_space.swap_tail_value(data)) {
        //     std::cout << "thread_alpha = " << static_cast<void *>(&data->matrix) << std::endl;

        //     std::cout << data->alpha << std::endl;
        //     std::cout << data->matrix[0][0] << std::endl;
        //     // std::cout << data->beta << std::endl;
        //     for (int x = 0; x < 15; x++) {
        // //         std::vector<char> tmp(15);
        //         for (int y = 0; y < 15; y++) {
        //             std::cout << data->matrix[x][y] << " ";
        //         }
        //         std::cout << std::endl;
        //     }


            // data->move->second = alphabeta(data->matrix, data->depth, data->alpha, data->beta, data->isAiTurn, data->playerBits, data->opponentBits);
            data->move->second = alphabeta(data->matrix, data->depth, data->alpha, data->beta, data->isAiTurn, data->playerBits, data->opponentBits);
            main_square_flag.unlock_shared();
            std::cout << "unlock" << std::endl;
        }
        // maybe sleep here;
        std::this_thread::sleep_for(std::chrono::milliseconds(2)); // need shorter sleep
        // std::this_thread::sleep_for(std::chrono::duration<float>(1)); // need shorter sleep

    //     if (!game_memory_space_working.empty() && game_memory_space_working.swap_tail_value(data)) {
    //         // compute
    //         // region basic compute
    //         if (checkWinner(data->opponentBits, data->depth)) { // WinChecker(board, (isAiTurn ? _AI_Color : _Enemy_Color)).result();
    //     //        std::cout << "Critical defeat! And a " << (isAiTurn ? " bad" : "good") << " one!!!" << std::endl;
    //             data->score = data->isAiTurn ? -9999 : 9999;
    //             data->gameStateProtector.unlock_shared();
    //             // break from everything
    //         }

    //         // stop at MAX_DEPTH
    //         if (data->depth >= MAX_DEPTH) {
    //             if (checkWinner(data->playerBits, data->depth)) {
    //                 data->score = data->isAiTurn ? 9999 : -9999;
    //                 data->gameStateProtector.unlock_shared();
    //                 // break from everything
    //             }
    //             else {
    //                 size_t eval = staticEval(data->playerBits) - staticEval(data->opponentBits);
    //                 data->score = data->isAiTurn ? eval : -eval;
    //                 data->gameStateProtector.unlock_shared();
    //                 // break from everything
    //             }
    //         }
    //         // endregion basic compute. preferably

    //         int best = data->isAiTurn ? -9999 : 9999;
    //         std::vector<std::pair<int, int>> squares = getSquaresToCheck(data->matrix);
    //         // endregion basic compute.

    //         if (data->depth >= MAX_DEPTH) {
    //             // compute alpha beta

    //         }
    //         else {
    //             // std::vector<GameState *>::iterator results_it;
    //             GameState *new_data = new GameState();

    //             for (size_t i = 0; i < squares.size(); i++) {
    //                 int y = squares[i].first;
    //                 int x = squares[i].second;
    //                 uint256_t pos = ((y * 15) + x);

    //                 data->matrix[y][x] = (data->isAiTurn ? -1 : 1);
    //                 data->playerBits |= 1 << pos;

    //                 // // //alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);
    //                 new_data->matrix = data->matrix; // may need a deep copy here.
    //                 new_data->depth = (data->depth + 1);
    //                 new_data->alpha = data->alpha;
    //                 new_data->beta = data->beta;
    //                 new_data->isAiTurn = !data->isAiTurn;
    //                 new_data->opponentBits = data->opponentBits;
    //                 new_data->playerBits = data->playerBits;
    //                 new_data->gameStateProtector.lock_shared();// ??? should we lock this one or the one one above in the tree ???
    //                 game_memory_space_working.swap_head_value(new_data);

    //                 // // data->results.push_back(new GameState());
    //                 // // results_it = data->results.end();
    //                 // // results_it--;
    //                 // // //alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);
    //                 // results_it->matrix = matrix; // may need a deep copy here.
    //                 // results_it->depth = (depth + 1);
    //                 // results_it->alpha = alpha;
    //                 // results_it->beta = beta;
    //                 // results_it->isAiTurn = !isAiTurn;
    //                 // results_it->opponentBits = opponentBits;
    //                 // results_it->playerBits = playerBits;
    //                 // results_it->gameStateProtector.lock_shared();// ??? should we lock this one or the one one above in the tree ???
    //                 game_memory_space_working.swap_head_value(new_data);

    // // std::list<ThreadPool>::iterator thread_it;

    // // for(int i = 0; i < THREAD_NUMBER; i++) {
    // //     thread_pool.push_back(ThreadPool());
    // //     thread_it = thread_pool.end();
    // //     thread_it--;
    // //     thread_it->parity_calculator_thread = std::thread(&ParityMaker::ParityCalculater, this, thread_it, i);
    // //     thread_it->parity_calculator_thread.detach();
    // // }

    //                 data->matrix[y][x] = 0;
    //                 data->playerBits &= ~(1 << pos);
    //             }
    //             delete new_data; // ??? can we save it ???
    //         }

    //         // for (size_t i = 0; i < squares.size(); i++) {
    //         //     // int y = squares[i].first;
    //         //     // int x = squares[i].second;
    //         //     // uint256_t pos = ((y * 15) + x);

    //         //     // matrix[y][x] = (isAiTurn ? -1 : 1);
    //         //     // playerBits |= 1 << pos;

    //         //     // int score = alphabeta(matrix, (depth + 1), alpha, beta, !isAiTurn, opponentBits, playerBits);

    //         //     // matrix[y][x] = 0;
    //         //     // playerBits &= ~(1 << pos);

    //         //     // std::cout << "For X:" << x << " et Y:" << y << " score:" << score << " for " << (isAiTurn ? "us" : "human") << std::endl;
    //         //     if (isAiTurn) {
    //         //         if (score >= beta) {
    //         //             return score;
    //         //         }

    //         //         best = std::max(score, best);
    //         //         alpha = std::max(alpha, score);
    //         //     }
    //         //     else {
    //         //         if (score <= alpha) {
    //         //             return score;
    //         //         }

    //         //         best = std::min(score, best);
    //         //         beta = std::min(beta, score);
    //         //     }

    //         //     if (score == 9999 && isAiTurn) {
    //         //         return score;
    //         //     }
    //         //     else if (score == -9999 && !isAiTurn) {
    //         //         return score;
    //         //     }
    //         // }

    //     }
    //     else if (!game_memory_space_waiting.empty() && game_memory_space_waiting.swap_tail_value(data)) {
    //         // compute
    //     }
    }
    delete data;
    return 0;
}

int AI::leaves_thread(std::list<ThreadPool>::iterator thread_it)
{
    return 0;
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

        std::cout << "Returning " << rsl.first << " " << rsl.second << std::endl;
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


//    data[0][0] = 1;
    data[0][1] = 1;
    data[0][2] = 1;
    data[0][3] = 1;
//    data[0][4] = 1;


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