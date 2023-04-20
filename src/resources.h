#ifndef RESOURCES_H
#define RESOURCES_H

#include <GLFW/glfw3.h>

void load_window_icon      (GLFWwindow *window, const char *icon_path);
unsigned int load_texture  (const char *texture_path);
unsigned int shader_program(const char *vertex_path, const char *fragment_path);

#endif
