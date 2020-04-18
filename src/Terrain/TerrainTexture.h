#pragma once

#include <iostream>

class TerrainTexture {
public:
    static unsigned int get() {
        static TerrainTexture instance;
        return instance.texture;
    }

    TerrainTexture(TerrainTexture&)=delete;
    TerrainTexture& operator=(TerrainTexture&)=delete;
    TerrainTexture(TerrainTexture&&)=delete;
    TerrainTexture& operator=(TerrainTexture&&)=delete;

private:
    unsigned int texture;

    TerrainTexture();
    ~TerrainTexture() = default;
};