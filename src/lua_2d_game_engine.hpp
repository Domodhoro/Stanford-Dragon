#ifndef LUA_2D_GAME_ENGINE_HPP
#define LUA_2D_GAME_ENGINE_HPP

namespace engine {

GLFWwindow *window {nullptr};

float get_key_number(lua_State *L, const char *key) {
    lua_pushstring(L, key);
    lua_gettable  (L, -2);

    const auto result {static_cast<float>(luaL_checknumber(L, -1))};

    lua_pop(L, 1);

    return result;
}

const char *get_key_string(lua_State *L, const char *key) {
    lua_pushstring(L, key);
    lua_gettable  (L, -2);

    const auto result {static_cast<const char*>(luaL_checkstring(L, -1))};

    lua_pop(L, 1);

    return result;
}

void load_window_icon(GLFWwindow *window, const char *icon_path) {
    GLFWimage img;

    img.pixels = stbi_load(icon_path, &img.width, &img.height, 0, 4);

    if (img.pixels == nullptr) error_log(__FILE__, __LINE__, "falha ao carregar ícone da janela de visualização");

    glfwSetWindowIcon(window, 1, &img);

    stbi_image_free(img.pixels);
}

int read_file(lua_State *L) {
    const auto file_path {luaL_checkstring(L, 1)};

    std::ifstream file     {file_path};
    std::stringstream code {};

    if (!file.is_open()) error_log(__FILE__, __LINE__, "falha ao abrir arquivo 'GLSL'");

    code << file.rdbuf();

    file.close();

    lua_pushstring(L, code.str().c_str());

    return 1;
}

int glfw_context_init(lua_State *L) {
    if (glfwInit() == GLFW_NOT_INITIALIZED) error_log(__FILE__, __LINE__, "falha ao iniciar 'glfw'");

    const auto version_major {luaL_checkinteger(L, 1)};
    const auto version_minor {luaL_checkinteger(L, 2)};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

int glfw_create_window(lua_State *L) {
    lua_getglobal(L, "window");

    const auto title  {get_key_string(L, "title")};
    const auto width  {static_cast<int>(get_key_number(L, "width"))};
    const auto height {static_cast<int>(get_key_number(L, "height"))};

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (window == nullptr) error_log(__FILE__, __LINE__, "falha ao criar janela de visualização");

    glfwMakeContextCurrent        (window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    const auto x {static_cast<int>(get_key_number(L, "x"))};
    const auto y {static_cast<int>(get_key_number(L, "y"))};

    glfwSetWindowPos       (window, x, y);
    glfwSetWindowSizeLimits(window, width, height, 800, 600);

    const auto icon_path {get_key_string(L, "icon_path")};

    load_window_icon(window, icon_path);

    glewExperimental = true;

    if (glewInit() != GLEW_OK) error_log(__FILE__, __LINE__, "falha ao iniciar 'glew'");

    return 0;
}

int gl_clear_buffers(lua_State *L) {
    lua_getglobal(L, "color");

    const auto red   {get_key_number(L, "red")};
    const auto green {get_key_number(L, "green")};
    const auto blue  {get_key_number(L, "blue")};

    glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(red, green, blue, 1.0f);

    return 0;
}

int glfw_window_should_close(lua_State *L) {
    lua_pushboolean(L, glfwWindowShouldClose(window));

    return 1;
}

int glfw_set_window_should_close(lua_State *L) {
    glfwSetWindowShouldClose(window, true);

    return 0;
}

int glfw_swap_buffers(lua_State *L) {
    glfwSwapBuffers(window);

    return 0;
}

int glfw_poll_events(lua_State *L) {
    glfwPollEvents();

    return 0;
}

int glfw_get_time(lua_State *L) {
    lua_pushnumber(L, glfwGetTime());

    return 1;
}

int glfw_get_key(lua_State *L) {
    const auto key {luaL_checkinteger(L, 1)};

    lua_pushboolean(L, (glfwGetKey(window, key) == GLFW_PRESS));

    return 1;
}

int glfw_context_terminate(lua_State *L) {
    glfwDestroyWindow(window);
    glfwTerminate    ();

    return 0;
}

}

#endif
