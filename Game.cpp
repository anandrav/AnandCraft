#include "Game.h"

const int TICKRATE = 20;
const int MS_PER_UPDATE = 1000 / TICKRATE;

Game::Game(SDL_Window* window_in) : player_controller(&player, &world), is_running(true), window(window_in) {
    // Create texture from image
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/texture-atlas.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Game::run_loop() {
    double previous = SDL_GetTicks();
    double lag = 0.0;
    while (is_running) {
        double current = SDL_GetTicks();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        while (lag >= MS_PER_UPDATE) {
            process_input();
            update();
            lag -= MS_PER_UPDATE;
        }

        const double interpolate = lag / MS_PER_UPDATE;
        render();
    }
}

void Game::process_input() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
        } else {
            player_controller.process_event(e);
        }
    }
}

void Game::update() {
    // TODO pack this behavior inside a Player class + Controller class

    player.update();
    world.update(player.get_position());

    AsyncQueue::get_instance().process_all_tasks();
}

void Game::render() {
    // clear
    glDepthMask(GL_TRUE);
    glClearColor(.60f, .70f, .95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render world
    glBindTexture(GL_TEXTURE_2D, texture);
    world.render_opaque(player.get_camera());
    world.render_transparent(player.get_camera());

    // render UI/HUD

    // update display
    SDL_GL_SwapWindow(window);
}