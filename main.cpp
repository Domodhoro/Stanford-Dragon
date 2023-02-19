/*
    MIT License

    Copyright (c) 2023 Guilherme M. Aguiar (guilhermemaguiar2022@gmail.com)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifdef __cplusplus

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"
}

#endif

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct my_exception {
    my_exception(const char *file, int line, const char *description) {
        puts  ("Ops! Uma falha ocorreu.");
        printf("File:        %s\n", file);
        printf("Line:        %i\n", line);
        printf("Description: %s\n", description);
    }

    ~my_exception() { exit(EXIT_FAILURE); }
};

constexpr auto FPS           {60};
constexpr auto WINDOW_WIDTH  {1300};
constexpr auto WINDOW_HEIGHT {800};
constexpr auto WINDOW_TITLE  {"Stanford Dragon"};
constexpr auto CAMERA_FOV    {72.0f};

enum struct CAMERA_MOVEMENTS : int {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT
};

#include "./src/camera.hpp"
#include "./src/dragon.hpp"
#include "./src/stb_image_wrapper.hpp"

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void keyboardCallback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main(int argc, char *argv[]) {
    puts(argv[0]);

    if (glfwInit() == GLFW_NOT_INITIALIZED) my_exception {__FILE__, __LINE__, "falha ao iniciar o GLFW"};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);

    auto window {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr)
    };

    if (window == nullptr) my_exception {__FILE__, __LINE__, "falha ao criar a janela de visualização"};

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto mode {glfwGetVideoMode(glfwGetPrimaryMonitor())};

    auto window_pos_x {(mode->width  - WINDOW_WIDTH)  / 2};
    auto window_pos_y {(mode->height - WINDOW_HEIGHT) / 2};

    glfwSetWindowPos(window, window_pos_x, window_pos_y);

    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    stb_image_wrapper::load_window_icon(window, "./img/icon.bmp");

    glewExperimental = true;

    if (glewInit() != GLEW_OK) my_exception {__FILE__, __LINE__, "falha ao iniciar GLEW"};

    camera::camera cam {WINDOW_WIDTH, WINDOW_HEIGHT};

    cam.set_FOV     (CAMERA_FOV);
    cam.set_position({0.0f, 0.0f, -2.5f});

    Dragon dragon {};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    float lastFrame = 0.0f, currentFrame = 0.0f, FPS = 60.0f;

    while (!glfwWindowShouldClose(window)) {
        currentFrame = static_cast<float>(glfwGetTime());

        if ((currentFrame - lastFrame) > (1.0f / FPS)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            glm::mat4 projectionMatrix = cam.get_projection_matrix();
            glm::mat4 viewMatrix       = cam.get_view_matrix();

            dragon.render(projectionMatrix, viewMatrix, {0.0f, 0.0f, -2.5f});

            glfwSwapBuffers(window);
            glfwPollEvents();

            lastFrame = currentFrame;
        }

        keyboardCallback(window);
    }

    glfwTerminate();

    return 0;
}
