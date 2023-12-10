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

    FILE *file = fopen("./ML_files/random-file", "rb"); // can fail randomly, need protection
    size_t tmp;

    if (file) {
        // fread(&parameters, sizeof(ML_Parameters), 1, file); // read parameters to file
        fread(&parameters.version, sizeof(int), 1, file); // read parameters to file
        fread(&parameters.turn_number, sizeof(unsigned int), 1, file); // read parameters to file

        if (parameters.version != ML_VERSION)
            exit(64); // maybe a bit extreme
        scores = std::vector<std::vector<ML_Node>>(parameters.turn_number);

        for (size_t i = 0; (i < parameters.turn_number && fread(&tmp, sizeof(size_t), 1, file)); i++) {
            // scores.push_back(std::vector<ML_Node>());
            std::cout << "tmp == " << tmp << std::endl;
            scores[i] = std::vector<ML_Node>(tmp);
            for (size_t j = 0; j < tmp; j++) {
                fread(&scores[i][j].boardHash, sizeof(XXH64_hash_t), 1, file);
                fread(&scores[i][j].score, sizeof(int), 1, file);
            }
        // //     fread(scores[i].data(), sizeof(ML_Node), tmp, file);
        }
        fclose(file);
    }
    else {
        parameters.version = ML_VERSION;
        parameters.turn_number = 0;
    }
    // parameters.version = ML_VERSION;
    // parameters.turn_number = 0;
}

ML::~ML()
{
    // write scores in files

    FILE *file = fopen("./ML_files/random-file", "wb"); // can fail randomly, need protection
    // ML_Parameters tt;
    // int *ok = new int;
    // *ok = 0;
    // int kk = 3;


    parameters.turn_number = scores.size();
    // fwrite(&ok, sizeof(int), 1, file); // write parameters to file
    // fwrite(&kk, sizeof(int), 1, file); // write parameters to file
    fwrite(&parameters.version, sizeof(int), 1, file); // write parameters to file
    fwrite(&parameters.turn_number, sizeof(unsigned int), 1, file); // write parameters to file
    // // // fwrite(&parameters.turn_number, sizeof(unsigned int), 1, file); // write parameters to file
    // // // // fwrite(&tt, sizeof(ML_Parameters), 1, file); // write parameters to file

    for (size_t i = 0; i < scores.size(); i++) {
        size_t tmp = scores[i].size();
        fwrite(&tmp, sizeof(size_t), 1, file); // write number of node for each turn.
    // // //     std::cout << "tmp = " << tmp << std::endl;
    // // //     ML_Node l = ML_Node();
    // // //     // fwrite(scores[i].data(), sizeof(ML_Node), tmp, file); // write all the nodes.
    // // //     // fwrite(&scores[i].front(), sizeof(ML_Node), tmp, file); // write all the nodes.
    // // //     // fwrite(&scores[i].front(), sizeof(ML_Node), 1, file); // write all the nodes.
        // fwrite(&scores[i], sizeof(ML_Node), 1, file); // write all the nodes.
        // fwrite(scores[i].data(), sizeof(ML_Node), tmp, file); // write all the nodes.
        // fwrite(scores[i].data(), sizeof(ML_Node), tmp, file); // write all the nodes.
    // // //     // if (tmp > 0)
    // // //     //     fwrite(&scores[i][0], sizeof(ML_Node), 1, file); // write all the nodes.
    // // //     fwrite(&l, sizeof(ML_Node), 1, file); // write all the nodes.
        for (size_t j = 0; j < tmp; j++) {
            XXH64_hash_t aa = scores[i][j].boardHash;
            int aaaa = scores[i][j].score;
            // fwrite(&aa, sizeof(XXH64_hash_t), 1, file); // write all the nodes.
            // fwrite(&aaaa, sizeof(int), 1, file); // write all the nodes.
            fwrite(&scores[i][j].boardHash, sizeof(XXH64_hash_t), 1, file); // write all the nodes.
            fwrite(&scores[i][j].score, sizeof(int), 1, file); // write all the nodes.
        }
    }
    fclose(file);
    // delete ok;

    // for (size_t i = 0; i < scores.size(); i++) {
    //     delete scores[i];
    // }

    // for (size_t i = 0; i < scores.size(); i++) {
    //     for
    // }

    // fread(&usr, sizeof(ML_Parameters), 1, fp)
    // unsigned int
}


std::pair<bool, int> ML::getScore(XXH64_hash_t &boardHash, int turn)
{
    if (turn > scores.size())
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
    while (scores.size() < (turn + 1))
        scores.push_back(std::vector<ML_Node>());
    scores[turn].push_back(tmp);
}