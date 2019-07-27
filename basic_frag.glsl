#version 330 core
out vec4 FragColor;

uniform sampler2D ourTexture;

in vec2 TexCoord;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    //FragColor = vec4(1, 0, 0, 1);
}