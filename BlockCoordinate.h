/*
Coordinate in World space in units of Blocks, which happen to be 1 unit wide.
Used for identifying blocks by location.
*/

#pragma once

struct BlockCoordinate {
    int x;
    int y;
    int z;
};

