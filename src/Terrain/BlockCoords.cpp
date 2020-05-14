#include "BlockCoords.h"

using namespace std;

#include "ChunkWidth.h"
#include "ChunkCoords.h"

int convert_float(float f) {
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
    : x(convert_float(vec3.x))
    , y(convert_float(vec3.y))
    , z(convert_float(vec3.z))
{
}

BlockCoords::BlockCoords(const ChunkCoords& chunk_coords)
    : x(chunk_coords.x * CHUNK_WIDTH)
    , y(chunk_coords.y * CHUNK_WIDTH)
    , z(chunk_coords.z * CHUNK_WIDTH)
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