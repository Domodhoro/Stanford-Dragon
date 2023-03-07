#ifdef __cplusplus

extern "C" {
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"

#include "./lua54/luaconf.h"
#include "./lua54/lua.h"
#include "./lua54/lualib.h"
#include "./lua54/lauxlib.h"
}

#endif

#include <vector>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr auto FPS           {60};
constexpr auto WINDOW_WIDTH  {800};
constexpr auto WINDOW_HEIGHT {500};
constexpr auto WINDOW_TITLE  {"Stanford Dragon"};
constexpr auto CAMERA_FOV    {72.0f};

template<typename T>
struct vertex {
    glm::tvec3<T> position;
    glm::tvec3<T> normal;
};

enum struct CAMERA_MOVEMENTS : int {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT
};

static void error_log(const char *file, const int line, const char *description) {
    puts  ("A failure occurred!");
    printf("File:        %s.\n", file);
    printf("Line:        %i.\n", line);
    printf("Description: %s.\n", description);

    exit(EXIT_FAILURE);
}

#include "./src/lua_script.hpp"
#include "./src/camera.hpp"
#include "./src/shader.hpp"
#include "./src/mesh.hpp"
#include "./src/dragon.hpp"

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
static void keyboard_callback        (GLFWwindow *window)                        { if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true); }

static void load_window_icon(GLFWwindow *window, const char *icon_path) {
    GLFWimage img;

    img.pixels = stbi_load(icon_path, &img.width, &img.height, 0, 4);

    if (img.pixels == nullptr) error_log(__FILE__, __LINE__, "falha ao carregar ícone da janela de visualização");

    glfwSetWindowIcon(window, 1, &img);

    stbi_image_free(img.pixels);
}

int main(int argc, char *argv[]) {
    puts(argv[0]);

    lua_script::lua_script lua("./script.lua");

    if (glfwInit() == GLFW_NOT_INITIALIZED) error_log(__FILE__, __LINE__, "falha ao iniciar o GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    auto window {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr)
    };

    if (window == nullptr) error_log(__FILE__, __LINE__, "falha ao criar a janela de visualiza��o");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto mode {glfwGetVideoMode(glfwGetPrimaryMonitor())};

    const auto window_pos_x {(mode->width  - WINDOW_WIDTH)  / 2};
    const auto window_pos_y {(mode->height - WINDOW_HEIGHT) / 2};

    glfwSetWindowPos(window, 100, 100);

    load_window_icon(window, "./img/icon.bmp");

    glewExperimental = true;

    if (glewInit() != GLEW_OK) error_log(__FILE__, __LINE__, "falha ao iniciar GLEW");

    camera::camera cam {WINDOW_WIDTH, WINDOW_HEIGHT};

    cam.set_FOV     (CAMERA_FOV);
    cam.set_position({0.0f, 0.0f, -2.0f});

    shader::shader_program shader {"./glsl/dragon_vertex.glsl", "./glsl/dragon_fragment.glsl"};

    dragon::dragon my_dragon {};

    my_dragon.set_light_position({-3.0f, 4.0f,-5.0f});
    my_dragon.set_light_color   ({ 0.3f, 0.8f, 0.4f});

    glEnable   (GL_DEPTH_TEST);
    glEnable   (GL_CULL_FACE);
    glFrontFace(GL_CCW);

    auto last_frame    {0.0f};
    auto current_frame {0.0f};

    while (!glfwWindowShouldClose(window)) {
        current_frame = glfwGetTime();

        if ((current_frame - last_frame) > (1.0f / FPS)) {
            glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

            keyboard_callback(window);

            glm::mat4 model {1.0f};

            model = rotate(model, glm::radians<float>(glfwGetTime() * 45.0f), {0.0f, 1.0f, 0.0f});

            my_dragon.render(model, shader, cam);

            glfwSwapBuffers (window);
            glfwPollEvents  ();
            glfwSetWindowPos(window, window_pos_x, window_pos_y);

            last_frame = current_frame;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate    ();

    return 0;
}
