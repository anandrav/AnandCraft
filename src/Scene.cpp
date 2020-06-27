#include "Scene.h"

#include "Entity.h"
#include "Player.h"
#include "Terrain/Terrain.h"
#include "BlockDemo.h"

using namespace std;

void Scene::register_entity(Entity* entity) 
{
    entities.insert(entity);
}

void Scene::deregister_entity(Entity* entity) 
{
    entities.erase(entity);
}

void Scene::update()
{
    for_each(begin(entities), end(entities), mem_fn(&Entity::update));
}

void Scene::render() const
{
    for_each(begin(entities), end(entities), [](Entity* e) { e->render_opaque(g_player->get_camera());});
    for_each(begin(entities), end(entities), [](Entity* e) { e->render_transparent(g_player->get_camera());});
}