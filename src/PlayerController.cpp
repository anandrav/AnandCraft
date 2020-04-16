#include "PlayerController.h"

#include "Ray.h"
#include <iostream>

const float VELOCITY = 0.6f;

PlayerController::PlayerController(Player* player_in, World* world_in) :
    player(player_in), world(world_in)
{
}

void PlayerController::process_event(SDL_Event& e)
{
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
    const float MAX_DISTANCE = 12.0f;
    const float STEP_DISTANCE = 0.1f;

    int prev_x_coord = int(player->get_position().x);
    int prev_y_coord = int(player->get_position().y);
    int prev_z_coord = int(player->get_position().z);

    // find the first block that isn't air and do something
    for (Ray ray(player->get_position(), player->get_forward());
        ray.get_length() < MAX_DISTANCE; ray.step(STEP_DISTANCE)) {

        // round positive and negative coordinates "toward zero"
        int x_coord = (int)ray.get_end().x - (ray.get_end().x < 0);
        int y_coord = (int)ray.get_end().y - (ray.get_end().y < 0);
        int z_coord = (int)ray.get_end().z - (ray.get_end().z < 0);

        // don't check the same block twice
        if (x_coord == prev_x_coord && y_coord == prev_y_coord && z_coord == prev_z_coord) {
            continue;
        }

        // if (world->has_block_at(x_coord, y_coord, z_coord)) {
        //     BlockData block = world->get_block_at(x_coord, y_coord, z_coord);
        //     if (block.id != BlockID::AIR) {
        //         if (e.button.button == SDL_BUTTON_LEFT) {
        //             // break block
        //             BlockData new_data = BlockData(BlockID::AIR);
        //             world->modify_block_at(x_coord, y_coord, z_coord, new_data);
        //         }
        //         if (e.button.button == SDL_BUTTON_RIGHT) {
        //             // place block
        //             BlockData new_data = BlockData(BlockID::PLANK);
        //             if (world->has_block_at(prev_x_coord, prev_y_coord, prev_z_coord)) {
        //                 world->modify_block_at(prev_x_coord, prev_y_coord, prev_z_coord, new_data);
        //             }
        //         }
        //         break;
        //     }
        // }

        prev_x_coord = x_coord;
        prev_y_coord = y_coord;
        prev_z_coord = z_coord;
    }
}