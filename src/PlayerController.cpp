#include "PlayerController.h"

#include "RaycastEvent.h"
#include "Game.h"

#include <iostream>
#include <memory>

using namespace std;

const float VELOCITY = 5.0f;

PlayerController::PlayerController(Player* player) 
    : player(player)
{
}

void PlayerController::process_event(SDL_Event& e)
{
    if (!player)
        return;
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w:
            if (player->get_velocity().z > 0.f) {
                player->set_velocity_z(0.0f);
            } else {
                player->set_velocity_z(-VELOCITY);
            }
            break;
        case SDLK_a:
            if (player->get_velocity().x > 0.f) {
                player->set_velocity_x(0.0f);
            } else {
                player->set_velocity_x(-VELOCITY);
            }
            break;
        case SDLK_s:
            if (player->get_velocity().z < 0.f) {
                player->set_velocity_z(0.0f);
            } else {
                player->set_velocity_z(VELOCITY);
            }
            break;
        case SDLK_d:
            if (player->get_velocity().x < 0.f) {
                player->set_velocity_x(0.0f);
            } else {
                player->set_velocity_x(VELOCITY);
            }
            break;
        case SDLK_SPACE:
            if (player->get_velocity().y < 0.f) {
                player->set_velocity_y(0.0f);
            } else {
                player->set_velocity_y(VELOCITY);
            }
            break;
        case SDLK_LSHIFT:
            if (player->get_velocity().y > 0.f) {
                player->set_velocity_y(0.0f);
            } else {
                player->set_velocity_y(-VELOCITY);
            }
            break;
        default:
            break;
        }
    }
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        case SDLK_w:
            if (player->get_velocity().z < 0.f) {
                player->set_velocity_z(0.0f);
            } else {
                player->set_velocity_z(VELOCITY);
            }
            break;
        case SDLK_a:
            if (player->get_velocity().x < 0.f) {
                player->set_velocity_x(0.0f);
            } else {
                player->set_velocity_x(VELOCITY);
            }
            break;
        case SDLK_s:
            if (player->get_velocity().z > 0.f) {
                player->set_velocity_z(0.0f);
            } else {
                player->set_velocity_z(-VELOCITY);
            }
            break;
        case SDLK_d:
            if (player->get_velocity().x > 0.f) {
                player->set_velocity_x(0.0f);
            } else {
                player->set_velocity_x(-VELOCITY);
            }
            break;
        case SDLK_SPACE:
            if (player->get_velocity().y > 0.f) {
                player->set_velocity_y(0.0f);
            } else {
                player->set_velocity_y(-VELOCITY);
            }
            break;
        case SDLK_LSHIFT:
            if (player->get_velocity().y < 0.f) {
                player->set_velocity_y(0.0f);
            } else {
                player->set_velocity_y(VELOCITY);
            }
            break;
        default:
            break;
        }
    }
    if (e.type == SDL_MOUSEMOTION) {
        player->pitch((-float(e.motion.yrel) / 1000.f));
        player->yaw((float(e.motion.xrel)) / 1000.f);
    }
    if (e.type == SDL_MOUSEBUTTONUP) {
        handle_click(e);
    }
}

void PlayerController::handle_click(SDL_Event& e)
{
    Ray ray(player->get_position(), player->get_forward());
    bool left_click = (e.button.button == SDL_BUTTON_LEFT);
    g_game->get_event_dispatcher().dispatch_event<RaycastEvent>(make_shared<RaycastEvent>(ray, left_click));
}