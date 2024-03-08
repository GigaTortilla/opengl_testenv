#version 330 core
layout (location = 0) in vec3 aPos;   		// the position variable has attribute position 0
layout (location = 1) in vec3 aColor; 		// the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoords;	// texture coordinates have the attribute position 2

out vec2 textureCoords;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	textureCoords = aTexCoords;
	ourColor = aColor;
} 