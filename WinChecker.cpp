#include "WinChecker.hpp"


WinChecker::WinChecker() {}

WinChecker::~WinChecker() {}

bool WinChecker::hasWon(const uint256_t &matrix)
{
    uint256_t h = 31;
    uint256_t v = 1152956690052710401;
    uint256_t d1 = 281479271743489; // 18447025552981295105
    d1 *= 65536;
    d1 += 1;
    uint256_t d2 = 1152991877646254096;

    if(matchBitmask(matrix, h)) return true;
    if(matchBitmask(matrix, d1)) return true;
    if(matchBitmask(matrix, d2)) return true;

    // vertical is a "bit" different :)
    while(v <= matrix) {
        if((v & matrix) == v) return true;
        v *= 2;
    }

    return false;
}

bool WinChecker::matchBitmask(const uint256_t &matrix, uint256_t &mask)
{
    for(size_t i = 0; mask <= matrix; i++, mask *= 2){
        if((i % 15) > 10) continue;
        if((mask & matrix) == mask) return true;
    }

    return false;
}