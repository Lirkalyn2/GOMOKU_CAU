#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include <vector>

#include "./uint256/uint256_t.h"

class WinChecker {
    public:
        WinChecker();
        ~WinChecker();
        static bool hasWon(const uint256_t &matrix);

    protected:
    private:
        static bool matchBitmask(const uint256_t &matrix, uint256_t &mask);
};

#endif /* !GOMOKU_HPP */
