#include <iostream>
#include "Game.h"

int main(int argc, char** argv) {
    std::cout << "AnandCraft 1.0\n";

    Game game;
    game.run_loop();

    return 0;
}