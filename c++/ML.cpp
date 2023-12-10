/*
** EPITECH PROJECT, 2023
** GOMOKU_CAU
** File description:
** ML
*/

#include "ML.hpp"

ML::ML()
{
    // check files parameters to make everything is alright
    // load scores into mem
}

ML::~ML()
{
    // write scores in files
}


std::pair<bool, int> ML::getScore(XXH64_hash_t &boardHash, int turn)
{
    // scores[turn].fi
    // std::cout << boardHash << std::endl;

    // std::find(vec.begin(), vec.end(), item) != vec.end()
    std::vector<std::pair<XXH64_hash_t, int>>::iterator rsl = std::find(scores[turn].begin(), scores[turn].end(), boardHash);

    if (rsl != scores[turn].end())
        return std::make_pair(true, rsl->second);
    //     return rsl->second;
    // return INT32_MIN;
    return std::make_pair(false, INT32_MIN);
}

void ML::putScore(XXH64_hash_t &boardHash, int turn, int &score)
{
    scores[turn].push_back(std::make_pair(boardHash, score));
}