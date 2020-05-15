#pragma once

#include <stdexcept>

struct SaveError : std::runtime_error {
    SaveError(const char* const msg) : std::runtime_error(msg) 
    {
    }
};