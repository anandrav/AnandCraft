#include "TerrainShader.h"

TerrainShader::TerrainShader() {
    shader = Shader("res/basic_vert.glsl", "res/basic_frag.glsl");
}