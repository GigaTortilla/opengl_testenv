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

#ifdef CAMDLL_EXPORTS /*  define ADD_EXPORTS *only* when building the DLL. */
  #define CAMDLL_API __declspec(dllexport)
#else
  #define CAMDLL_API __declspec(dllimport)
#endif

/* Define calling convention in one place, for convenience. */
#define CALL __cdecl

CAMDLL_API void CALL updateCam(Camera* cam, GLFWwindow* window, float frameDiff);
CAMDLL_API vec3s CALL moveForward(Camera* cam);
CAMDLL_API vec3s CALL moveRight(Camera* cam);
CAMDLL_API vec3s CALL moveBackward(Camera* cam);
CAMDLL_API vec3s CALL moveLeft(Camera* cam);
CAMDLL_API vec3s CALL moveDown(Camera* cam);
CAMDLL_API vec3s CALL moveUp(Camera* cam);

#endif