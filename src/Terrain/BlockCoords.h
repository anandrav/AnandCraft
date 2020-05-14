/*
 * Coordinate in World space in units of Blocks, which happen to be 1 unit wide.
 * Used for identifying blocks by location.
 */
#pragma once

#include <glm/glm.hpp>
#include <ostream>

struct ChunkCoords;

struct BlockCoords {
    BlockCoords(int x, int y, int z);

    // convenient conversion from glm::vec3 of float
    BlockCoords(const glm::vec3& vec3);

    // conversion from ChunkCoords to BlockCoords, "floored" toward the origin of the chunk
    BlockCoords(const ChunkCoords& chunk_coords);

    bool operator==(const BlockCoords& other) const;

    friend std::ostream& operator<<(std::ostream& os, const BlockCoords& coords);

    int x;
    int y;
    int z;
};

std::ostream& operator<<(std::ostream& os, const BlockCoords& coords);