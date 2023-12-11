/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** ML
*/

#ifndef ML_HPP
#define ML_HPP

#include <functional>
#include <iostream>
#include <algorithm>
#include <vector>

#include "./xxhashlib/xxhash.h"

#define ML_VERSION 1

struct ML_Parameters {
    int version = 0;
    unsigned int turn_number = 0;
    // depth
    //...
};

struct ML_Node {
    XXH64_hash_t boardHash = 0;
    int score = 0;
};

class ML {
    public:
        ML();
        ~ML();
        std::pair<bool, int> getScore(XXH64_hash_t &boardHash, int turn); // (false, -2B) if unknow
        void putScore(XXH64_hash_t &boardHash, int turn, int &score);

    protected:
    private:
        ML_Parameters parameters;
        std::vector<std::vector<ML_Node>> scores;
};

#endif /* !ML_HPP */
