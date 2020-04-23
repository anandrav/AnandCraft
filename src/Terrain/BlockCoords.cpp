#include "BlockCoords.h"

using namespace std;

int convert(float f) {
    // since rounding truncates (always floors), add 1 if negative
    return int(f) - (f < 0);
}

BlockCoords::BlockCoords(int x, int y, int z)
    : x(x)
    , y(y)
    , z(z)
{
}

BlockCoords::BlockCoords(const glm::vec3& vec3)
    : x(convert(vec3.x))
    , y(convert(vec3.y))
    , z(convert(vec3.z))
{
}

bool BlockCoords::operator==(const BlockCoords& other) const
{
    return (x == other.x &&
            y == other.y &&
            z == other.z
    );
}

ostream& operator<<(ostream& os, const BlockCoords& coords) {
    os << '(' << coords.x << ", " << coords.y << ", " << coords.z << ')';
    return os;
}