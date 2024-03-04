#version 330 core
layout (location = 0) in vec3 aPos;   		// the position variable has attribute position 0
layout (location = 1) in vec3 aNormalVec;   // the normal vector to the vertices has attribute position 1

out vec3 normalVec;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 modelInv;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // multiplying the transpose of the inverse of the upper left 3x3 part of the 4x4 model matrix
    // with the normal vector transforms it to world space
    normalVec = mat3(modelInv) * aNormalVec;
    fragPos = vec3(model * vec4(aPos, 1.0));
} 