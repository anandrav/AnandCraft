#include "util.h"

namespace util {

    void output_error_msg(const string& msg, int line = -1) {
        std::cout << "Error : " << msg << std::endl;
        if (line != -1) {
            std::cout << "\nLine : " << line << std::endl;
        }
        SDL_ClearError();
    }

    void check_SDL_error(int line = -1) {
        std::string error = SDL_GetError();

        if (error != "") {
            std::cout << "SDL Error : " << error << std::endl;
            if (line != -1) {
                std::cout << "\nLine : " << line << std::endl;
            }
            SDL_ClearError();
        }
    }

    int positive_modulo(int i, int n) {
        return (i % n + n) % n;
    }

}