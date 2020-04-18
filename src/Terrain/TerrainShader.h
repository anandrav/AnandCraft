#pragma once

#include "../Graphics/Shader.h"

class TerrainShader {
public:
    static GLuint ID() {
        static TerrainShader instance;
        return instance.shader.ID;
    }

    TerrainShader(TerrainShader&)=delete;
    TerrainShader& operator=(TerrainShader&)=delete;
    TerrainShader(TerrainShader&&)=delete;
    TerrainShader& operator=(TerrainShader&&)=delete;

private:
    TerrainShader();
    ~TerrainShader() = default;

    Shader shader;
};