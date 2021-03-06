#version 460 core

layout (location= 0) in vec3 aPos;
layout (location= 1) in vec3 aColor;
layout (location= 2) in vec2 aTexCoords;

out vec4 vertexColor;
out vec2 texCoords;


void main()
{
	gl_Position = vec4(aPos, 1.0f);
	vertexColor = vec4(aColor, 1.0f);
	texCoords = aTexCoords;
}