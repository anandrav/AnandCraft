#include "BlockCoords.h"

BlockCoords::BlockCoords(int x, int y, int z)
    : x(x)
    , y(y)
    , z(z)
{
}

BlockCoords::BlockCoords(const glm::vec3& vec3)
    : x(int(vec3.x))
    , y(int(vec3.y))
    , z(int(vec3.z))
{
}