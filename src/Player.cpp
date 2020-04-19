#include "Player.h"

#include "globals.h"

const float ASPECT_RATIO = float(WIDTH) / HEIGHT;

Player::Player() 
    : velocity(glm::vec3(0.f,0.f,0.f))
    , camera(Camera(ASPECT_RATIO))
{
}

void Player::update()
{
    camera.move_forward(-velocity.z);
    camera.move_left(-velocity.x);
    camera.move_up(velocity.y);
}