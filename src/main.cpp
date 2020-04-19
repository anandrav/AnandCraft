#include <iostream>
#include "Game.h"
#include "globals.h"
#include "util.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace std;

int main()
{
    cout << "AnandCraft" << endl;
    try {
        // initialize Game
        g_game = new Game();
        g_game->run();
        delete g_game;
    } catch (exception& error) {
        cout << "exception caught: " << error.what() << endl;
    } catch (...) {
        cout << "Unknown exception caught!" << endl;
    }
    return 0;
}