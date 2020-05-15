#include <iostream>
#include "Game.h"
#include "Scene.h"
#include "globals.h"
#include "util.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace std;

int main()
{
    cout << "AnandCraft" << endl;
    try {
        // initialize Game and Scene
        g_game = new Game();
        Scene* scene = new Scene();
        g_game->loop();
        delete scene;
        delete g_game;
    } catch (exception& error) {
        cout << "std::exception caught: " << error.what() << endl;
    } catch (...) {
        cout << "Unknown exception caught!" << endl;
    }
    return 0;
}