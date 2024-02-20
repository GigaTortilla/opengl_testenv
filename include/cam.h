#ifndef CAM_H
#define CAM_H

#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/struct.h>

#define CAM_SPEED 2.5f

typedef struct
{
    vec3s pos;
    vec3s front;
    vec3s right;
    vec3s up;

    float fov;
    float speed;

    bool firstMouse;
    float lastMouseX;
    float lastMouseY;

    float yawAngle;
    float pitchAngle;
} Camera;

void updateCam(Camera* cam, GLFWwindow* window, float frameDiff);
vec3s moveForward(Camera* cam);
vec3s moveRight(Camera* cam);
vec3s moveBackward(Camera* cam);
vec3s moveLeft(Camera* cam);

#endif