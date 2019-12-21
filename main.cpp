#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "AnandCraft" << endl;

    Game::instance().run_loop();

    cout << "Done" << endl;

    return 0;
}