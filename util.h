#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>

using std::string;

namespace util {

    void output_error_msg(const string& msg, int line);

    void check_SDL_error(int line);

    int positive_modulo(int i, int n);

}