#version 460 core

out vec4 pixColor;
in vec4 vertexColor;
in vec2 texCoords;

//uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    pixColor = texture(u_Texture, texCoords);
} 