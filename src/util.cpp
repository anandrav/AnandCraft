#include "util.h"

using namespace std;

namespace util {

    void output_error_msg(const string& msg, int line = -1) {
        std::cout << "Error : " << msg << std::endl;
        if (line != -1) {
            std::cout << "\nLine : " << line << std::endl;
        }
        SDL_ClearError();
    }

}