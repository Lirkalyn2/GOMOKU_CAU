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
        bool isElement(XXH64_hash_t &boardHash, int turn);
        void readFile(std::string fileName);
        void writeFile(std::string fileName);

        std::vector<std::vector<ML_Node>> scores;
    protected:
    private:
        ML_Parameters parameters;
        // bool _manual_mode = false;s
};

#endif /* !ML_HPP */
