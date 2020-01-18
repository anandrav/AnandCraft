#include <iostream>
#include "Game.h"
#include "globals.h"

using namespace std;

void set_OpenGL_attributes();

int main(int argc, char** argv)
{
    cout << "AnandCraft" << endl;

    try {
        SDL_Window* main_window;
        SDL_GLContext main_context;

        // Set up SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "Failed to init SDL\n";
        }
        SDL_SetRelativeMouseMode(SDL_TRUE);

        set_OpenGL_attributes();

        // create our window centered at WIDTH x HEIGHT resolution
        main_window = SDL_CreateWindow(
            "AnandCraft",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_OPENGL
        );
        if (!main_window) {
            std::cout << "Unable to create window\n";
            util::check_SDL_error(__LINE__);
        }

        // Set up OpenGL
        main_context = SDL_GL_CreateContext(main_window);
        if (!main_context) {
            util::check_SDL_error(__LINE__);
        }

        // vertical sync
        SDL_GL_SetSwapInterval(1);

        // init GLEW
        glewExperimental = GL_TRUE;
        glewInit();

        Game game(main_window);
        game.run_loop();

        // Clean up SDL and OpenGL
        SDL_GL_DeleteContext(main_context);
        SDL_DestroyWindow(main_window);
        SDL_Quit();

    } catch (exception& error) {
        cout << "std::exception caught: " << error.what() << endl;
    } catch (...) {
        cout << "Unknown exception caught!" << endl;
    }

    return 0;
}

void set_OpenGL_attributes()
{
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Use version 3.2 (modern OpenGL)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}