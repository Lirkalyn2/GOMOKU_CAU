#include "Game.hpp"

int main(int argc, char const *argv[])
{
    if (argc >= 2 && argv[1][0] == 'A' && argv[1][1] == 'I') {
        if (argc == 3) {
            Game().LaunchXAIGame(std::stoi(argv[2]));
        }
        else
            Game().LaunchXAIGame(10);
    }
    else
        Game().LaunchPlayerGame();
}
