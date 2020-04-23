#include "Player.h"

#include "globals.h"

Player::Player() 
    : velocity(glm::vec3(0.f))
{
}

void Player::update()
{
    camera.move_forward(-velocity.z);
    camera.move_left(-velocity.x);
    camera.move_up(velocity.y);
}