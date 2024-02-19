#ifndef TESTENV_FUNCS_H
#define TESTENV_FUNCS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void setWindowIcon(GLFWwindow* window, const char* path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void checkESC(GLFWwindow *window);
GLFWwindow* initWindow(int width, int height, const char* icon, const char* name);

int f_blending();
int f_hello_triangle();
int f_practice_triangle();
int f_tex_tri();
int f_tri_insanity();
int f_coord_sys();
int f_cubes();

#endif