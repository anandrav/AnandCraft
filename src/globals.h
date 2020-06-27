#pragma once

class Game;
class Scene;
class EventDispatcher;
class SceneCoordinator;
class Player;

extern Game* g_game;
extern Scene* g_scene;
extern EventDispatcher* g_event_dispatcher;
extern SceneCoordinator* g_scene_coordinator;
extern Player* g_player;

const int WIDTH = 1280;
const int HEIGHT = 720;