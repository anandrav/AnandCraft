#include "util.h"

namespace util {

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


}