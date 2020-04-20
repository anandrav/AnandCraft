/*
Coordinate in World space in units of Blocks, which happen to be 1 unit wide.
Used for identifying blocks by location.
*/

#pragma once

#include <glm/glm.hpp>

struct BlockCoords {
    BlockCoords(int x, int y, int z);

    // convenient conversion from glm::vec3 of floats
    BlockCoords(const glm::vec3& vec3);

    int x;
    int y;
    int z;
};

