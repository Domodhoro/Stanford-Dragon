#ifdef __cplusplus

extern "C" {
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./lua54/luaconf.h"
#include "./lua54/lua.h"
#include "./lua54/lualib.h"
#include "./lua54/lauxlib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"
}

#endif

#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void error_log(const char *file, const int line, const char *description) {
    puts  ("A failure occurred!");
    printf("File:        %s.\n", file);
    printf("Line:        %i.\n", line);
    printf("Description: %s.\n", description);

    exit(EXIT_FAILURE);
}

#include "./src/lua_2d_game_engine.hpp"

int main(int argc, char *argv[]) {
    puts(argv[0]);

    const auto L {luaL_newstate()};

    luaL_openlibs(L);

    lua_register(L, "glfw_context_init", engine::glfw_context_init);
    lua_register(L, "glfw_create_window", engine::glfw_create_window);
    lua_register(L, "glfw_window_should_close", engine::glfw_window_should_close);
    lua_register(L, "gl_clear_buffers", engine::gl_clear_buffers);
    lua_register(L, "glfw_swap_buffers", engine::glfw_swap_buffers);
    lua_register(L, "glfw_poll_events", engine::glfw_poll_events);
    lua_register(L, "glfw_get_time", engine::glfw_get_time);
    lua_register(L, "glfw_get_key", engine::glfw_get_key);
    lua_register(L, "glfw_set_window_should_close", engine::glfw_set_window_should_close);
    lua_register(L, "glfw_context_terminate", engine::glfw_context_terminate);
    lua_register(L, "read_file", engine::read_file);

    if (luaL_dofile(L, "./script.lua") != LUA_OK) error_log(__FILE__, __LINE__, "falha ao abrir/ler script lua");

    lua_getglobal (L, "main");
    lua_isfunction(L, -1);
    lua_pcall     (L, 0, 0, 0);

    if (L != nullptr) lua_close(L);

    return 0;
}
