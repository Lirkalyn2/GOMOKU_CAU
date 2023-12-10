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

#include "./xxhashlib/xxhash.h"

class ML {
    public:
        ML();
        ~ML();
        std::pair<bool, int> getScore(XXH64_hash_t &boardHash, int turn); // (false, -2B) if unknow
        void putScore(XXH64_hash_t &boardHash, int turn, int &score);
        // read file
        // write file

    protected:
    private:
    std::vector<std::vector<std::pair<XXH64_hash_t, int>>> scores;
    // structe to sotre hashes and resulting scores. (separet it by amount of turned passed)
};

#endif /* !ML_HPP */
