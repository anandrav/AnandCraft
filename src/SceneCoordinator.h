#pragma once

class Terrain;
class Player;
class SingleBlockDemo;

class SceneCoordinator {
public:
    void populate();

    void destroy();

private:
    Terrain* terrain;
    SingleBlockDemo* demo;
};