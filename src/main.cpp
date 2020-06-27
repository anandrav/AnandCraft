#include <iostream>
#include "Game.h"
#include "Scene.h"
#include "globals.h"
#include "SceneCoordinator.h"

using namespace std;

int main()
{
    cout << "AnandCraft" << endl;
    try {
        // initialize Game then Scene, populate with game objects, run game loop
        g_game = new Game();
        g_scene = new Scene();
        g_scene_coordinator = new SceneCoordinator();
        g_event_dispatcher = new EventDispatcher();
        g_scene_coordinator->populate();
        g_game->run();
        // game loop finished so scene was deleted, delete game
        delete g_game;
    } catch (exception& error) {
        cout << "std::exception caught: " << error.what() << endl;
    } catch (...) {
        cout << "Unknown exception caught!" << endl;
    }
    return 0;
}