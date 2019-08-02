#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>

using std::string;

namespace util {
    void check_SDL_error(int line);
}