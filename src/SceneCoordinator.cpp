#include "SceneCoordinator.h"

#include "globals.h"
#include "Player.h"
#include "Terrain/Terrain.h"
#include "BlockDemo.h"

void SceneCoordinator::populate()
{
    g_player = new Player();
    terrain = new Terrain(g_player);
    demo = new SingleBlockDemo();
    g_player->get_transform().translate({0,1,0});
}

void SceneCoordinator::destroy() {
    delete demo;
    delete terrain;
    delete g_player;
}