#include "Scene.h"

using namespace std;

Scene::Scene()
    : terrain(&g_game->get_player())
{
    g_game->get_player().get_transform().translate({0,1,0});
}