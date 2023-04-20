#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

#include "./resources.h"

void load_window_icon(GLFWwindow *window, const char *icon_path) {
    GLFWimage img;

    img.pixels = stbi_load(icon_path, &img.width, &img.height, 0, 4);

    if (img.pixels == NULL) exit(EXIT_FAILURE);

    glfwSetWindowIcon(window, 1, &img);
    stbi_image_free  (img.pixels);
}

unsigned int load_texture(const char *texture_path) {
    unsigned int texture = 0u;

    glGenTextures  (1, &texture);
    glBindTexture  (GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    int width    = 0;
    int height   = 0;
    int channels = 0;

    unsigned char *pixels = stbi_load(texture_path, &width, &height, &channels, 0);

    if (pixels == NULL) exit(EXIT_FAILURE);

    glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);

    return texture;
}

unsigned int shader_program(const char *vertex_path, const char *fragment_path) {
    int success = 1;

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource (vertex, 1, &vertex_path, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (success == 0) exit(EXIT_FAILURE);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource (fragment, 1, &fragment_path, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (success == 0) exit(EXIT_FAILURE);

    unsigned int shader = glCreateProgram();

    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram (shader);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}
