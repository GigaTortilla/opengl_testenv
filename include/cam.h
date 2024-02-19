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

void cam_updatePos(Camera* cam, GLFWwindow* window);
vec3s cam_moveForward(Camera* cam);
vec3s cam_moveRight(Camera* cam);
vec3s cam_moveBackward(Camera* cam);
vec3s cam_moveLeft(Camera* cam);

#endif