/*
** EPITECH PROJECT, 2023
** eee
** File description:
** ThreadPool
*/

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <thread>
#include <mutex>

// #define THREAD_MEMORY_SLOT 3
// #define THREAD_MEMORY
// #define THREAD_NUMBER 8

class ThreadPool {
    public:
        bool is_alive = true;
        std::thread game_calculator_thread;
        std::mutex *stop; // wan't have mutex here ???
        //int status = 0; //-1 = useless, 0 = default, 1 = stream watchin, 2 = stream uploading.
        // working, waiting, stoped
};

#endif /* !THREAD_POOL_HPP */
