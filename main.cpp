#ifdef __cplusplus

extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "./lua54/lua.h"
#include "./lua54/lualib.h"
#include "./lua54/lauxlib.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./src/resources.h"
}

#endif

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./src/mesh.hpp"

static int initialization       (lua_State *L);
static int create_window        (lua_State *L);
static int set_window_icon      (lua_State *L);
static int activate_extension   (lua_State *L);
static int window_should_close  (lua_State *L);
static int get_time             (lua_State *L);
static int clear_color          (lua_State *L);
static int swap_buffers         (lua_State *L);
static int get_key              (lua_State *L);
static int poll_events          (lua_State *L);
static int destroy_window       (lua_State *L);
static int terminate            (lua_State *L);
static int create_mesh          (lua_State *L);
static int create_shader_program(lua_State *L);
static int create_texture       (lua_State *L);
static int set_aspect           (lua_State *L);
static int draw                 (lua_State *L);
static int set_translation      (lua_State *L);
static int set_rotation         (lua_State *L);
static int set_scale            (lua_State *L);

int main(int argc, char *argv[]) {
    puts(argv[0]);

    auto L {luaL_newstate()};

    luaL_openlibs(L);

    auto new_lib = [](lua_State *L) -> int {
        const luaL_Reg engine[] = {
            {"initialization",        initialization},
            {"create_window",         create_window},
            {"set_window_icon",       set_window_icon},
            {"activate_extension",    activate_extension},
            {"window_should_close",   window_should_close},
            {"get_time",              get_time},
            {"clear_color",           clear_color},
            {"swap_buffers",          swap_buffers},
            {"get_key",               get_key},
            {"poll_events",           poll_events},
            {"destroy_window",        destroy_window},
            {"terminate",             terminate},
            {"create_mesh",           create_mesh},
            {"create_shader_program", create_shader_program},
            {"create_texture",        create_texture},
            {"set_aspect",            set_aspect},
            {"draw",                  draw},
            {"set_translation",       set_translation},
            {"set_rotation",          set_rotation},
            {"set_scale",             set_scale},

            {nullptr, nullptr}
        };

        luaL_newlib(L, engine);

        return 1;
    };

    luaL_requiref(L, "engine", new_lib, 1);

    if (luaL_dofile(L, "./script.lua") != LUA_OK) {
        puts("Arquivo 'script.lua' não encontrado!");

        sleep(5);
        exit (EXIT_FAILURE);
    }

    if (L != nullptr) lua_close(L);

    return 0;
}

static int initialization(lua_State *L) {
    if (glfwInit() == GLFW_NOT_INITIALIZED) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, luaL_checkinteger(L, 1));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, luaL_checkinteger(L, 2));

    return 0;
}

static int create_window(lua_State *L) {
    auto title  {luaL_checkstring (L, 1)};
    auto width  {luaL_checkinteger(L, 2)};
    auto height {luaL_checkinteger(L, 3)};

    auto window {
        glfwCreateWindow(width, height, title, nullptr, nullptr)
    };

    if (window == nullptr) exit(EXIT_FAILURE);

    glfwMakeContextCurrent(window);
    glfwSetWindowAttrib   (window, GLFW_RESIZABLE, false);

    auto framebuffer_size_callback = [](GLFWwindow *window, int width, int height) -> void {
        glViewport(0, 0, width, height);
    };

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto mode {glfwGetVideoMode(glfwGetPrimaryMonitor())};

    auto x {(mode->width  - width)  / 2};
    auto y {(mode->height - height) / 2};

    glfwSetWindowPos(window, x, y);

    lua_pushlightuserdata(L, window);

    return 1;
}

static int set_window_icon(lua_State *L) {
    auto window    {static_cast<GLFWwindow *>(lua_touserdata(L, 1))};
    auto icon_path {luaL_checkstring (L, 2)};

    load_window_icon(window, icon_path);

    return 0;
}

static int activate_extension(lua_State *L) {
    glewExperimental = true;

    if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);

    return 0;
}

static int window_should_close(lua_State *L) {
    auto window {static_cast<GLFWwindow *>(lua_touserdata(L, 1))};

    lua_pushboolean(L, glfwWindowShouldClose(window));

    return 1;
}

static int get_time(lua_State *L) {
    lua_pushnumber(L, glfwGetTime());

    return 1;
}

static int clear_color(lua_State *L) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto red   {static_cast<float>(luaL_checknumber(L, 1))};
    auto green {static_cast<float>(luaL_checknumber(L, 2))};
    auto blue  {static_cast<float>(luaL_checknumber(L, 3))};

    glClearColor(red, green, blue, 1.0f);

    return 0;
}

static int swap_buffers(lua_State *L) {
    auto window {static_cast<GLFWwindow *>(lua_touserdata(L, 1))};

    glfwSwapBuffers(window);

    return 0;
}

static int get_key(lua_State *L) {
    auto window {static_cast<GLFWwindow *>(lua_touserdata(L, 1))};

    lua_pushboolean(L, (glfwGetKey(window, luaL_checkinteger(L, 2)) == GLFW_PRESS));

    return 1;
}

static int poll_events(lua_State *L) {
    glfwPollEvents();

    return 0;
}

static int destroy_window(lua_State *L) {
    auto window {static_cast<GLFWwindow *>(lua_touserdata(L, 1))};

    if (window != nullptr) glfwDestroyWindow(window);

    return 0;
}

static int terminate(lua_State *L) {
    glfwTerminate();

    return 0;
}

static int create_mesh(lua_State *L) {
    std::vector<vertex> vertices {
        {{-0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f,-0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,-0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, 0.5f}, {1.0f, 1.0f}}
    };

    std::vector<unsigned int> indices {0u, 1u, 3u, 1u, 2u, 3u};

    auto Mesh_ptr {std::make_unique<Mesh>(vertices, indices)};

    lua_pushlightuserdata(L, Mesh_ptr.get());

    Mesh_ptr.release();

    return 1;
}

static int create_shader_program(lua_State *L) {
    auto Mesh_ptr {static_cast<Mesh *>(lua_touserdata(L, 1))};

    Mesh_ptr->set_shader(shader_program(luaL_checkstring(L, 2), luaL_checkstring(L, 3)));

    return 0;
}

static int create_texture(lua_State *L) {
    auto Mesh_ptr {static_cast<Mesh *>(lua_touserdata(L, 1))};

    Mesh_ptr->set_texture(load_texture(luaL_checkstring(L, 2)));

    return 0;
}

static int set_aspect(lua_State *L) {
    lua_pushnumber(L, luaL_checknumber(L, 1));
    lua_setglobal (L, "aspect");

    return 0;
}

static int draw(lua_State *L) {
    lua_getglobal(L, "aspect");

    auto aspect   {lua_isnumber(L, -1) ? static_cast<float>(lua_tonumber(L, -1)) : 1.0f};
    auto Mesh_ptr {static_cast<Mesh *>(lua_touserdata(L, 1))};

    Mesh_ptr->draw(glm::ortho<float>(-aspect, aspect, -1.0f, 1.0f));

    return 0;
}

static int set_translation(lua_State *L) {
    auto Mesh_ptr {static_cast<Mesh *>(lua_touserdata(L, 1))};

    auto x {static_cast<float>(luaL_checknumber(L, 2))};
    auto y {static_cast<float>(luaL_checknumber(L, 3))};

    Mesh_ptr->set_translation(glm::tvec2<float>(x, y));

    return 0;
}

static int set_rotation(lua_State *L) {
    auto Mesh_ptr {static_cast<Mesh *>(lua_touserdata(L, 1))};

    Mesh_ptr->set_rotation(static_cast<float>(luaL_checknumber(L, 2)));

    return 0;
}

static int set_scale(lua_State *L) {
    auto Mesh_ptr {static_cast<Mesh *>(lua_touserdata(L, 1))};

    Mesh_ptr->set_scale(static_cast<float>(luaL_checknumber(L, 2)));

    return 0;
}
