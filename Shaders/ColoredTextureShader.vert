#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoords;

out vec3 ourColor;
out vec2 textCoord;

void main()
{
	ourColor = color;
	gl_Position = vec4(position, 1.0);
	textCoord = textureCoords;
}