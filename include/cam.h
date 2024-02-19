#ifndef CAM_H
#define CAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/struct.h>

#define CAM_SPEED 0.05f

typedef struct
{
    vec3s pos;
    vec3s front;
    vec3s up;
    vec3s right;
    float speed;
} Camera;

void updatePos(Camera* cam, GLFWwindow* window);
vec3s moveForward(Camera* cam);
vec3s moveRight(Camera* cam);
vec3s moveBackward(Camera* cam);
vec3s moveLeft(Camera* cam);

#endif