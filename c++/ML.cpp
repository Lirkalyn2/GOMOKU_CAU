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

    FILE *file = fopen("./ML_files/random-file", "rb"); // can fail randomly, need protection
    size_t tmp;

    if (file) {
        fread(&parameters.version, sizeof(int), 1, file); // read parameters to file
        fread(&parameters.turn_number, sizeof(unsigned int), 1, file); // read parameters to file

        if (parameters.version != ML_VERSION)
            exit(64); // maybe a bit extreme
        scores = std::vector<std::vector<ML_Node>>(parameters.turn_number);

        for (size_t i = 0; (i < parameters.turn_number && fread(&tmp, sizeof(size_t), 1, file)); i++) {
            scores[i] = std::vector<ML_Node>(tmp);
            for (size_t j = 0; j < tmp; j++) {
                fread(&scores[i][j].boardHash, sizeof(XXH64_hash_t), 1, file);
                fread(&scores[i][j].score, sizeof(int), 1, file);
            }
        }
        fclose(file);
    }
    else {
        parameters.version = ML_VERSION;
        parameters.turn_number = 0;
    }
}

ML::~ML()
{
    FILE *file = fopen("./ML_files/random-file", "wb"); // can fail randomly, need protection

    parameters.turn_number = scores.size();
    fwrite(&parameters.version, sizeof(int), 1, file); // write parameters to file
    fwrite(&parameters.turn_number, sizeof(unsigned int), 1, file); // write parameters to file

    for (size_t i = 0; i < scores.size(); i++) {
        size_t tmp = scores[i].size();
        fwrite(&tmp, sizeof(size_t), 1, file); // write number of node for each turn.

        for (size_t j = 0; j < tmp; j++) {
            fwrite(&scores[i][j].boardHash, sizeof(XXH64_hash_t), 1, file);
            fwrite(&scores[i][j].score, sizeof(int), 1, file);
        }
    }
    fclose(file);
}


std::pair<bool, int> ML::getScore(XXH64_hash_t &boardHash, int turn)
{
    if (turn > (int)scores.size())
        return std::make_pair(false, INT32_MIN);
    for (std::vector<ML_Node>::iterator it = scores[turn].begin(); it != scores[turn].end(); it++)
        if (it != scores[turn].end() && it->boardHash == boardHash)
            return std::make_pair(true, it->score);
    return std::make_pair(false, INT32_MIN);
}

void ML::putScore(XXH64_hash_t &boardHash, int turn, int &score)
{
    ML_Node tmp;
    std::vector<ML_Node> mm;

    tmp.boardHash = boardHash;
    tmp.score = score;
    while ((int)scores.size() < (turn + 1))
        scores.push_back(std::vector<ML_Node>());
    scores[turn].push_back(tmp);
}