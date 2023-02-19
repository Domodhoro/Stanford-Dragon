#ifndef STB_IMAGE_WRAPPER_HPP
#define STB_IMAGE_WRAPPER_HPP

namespace stb_image_wrapper {

void load_window_icon(GLFWwindow *window, const char *icon_path) {
    GLFWimage img;

    img.pixels = stbi_load(icon_path, &img.width, &img.height, 0, 4);

    if (img.pixels == nullptr) my_exception {__FILE__, __LINE__, "falha ao carregar ícone da janela de visualização"};

    glfwSetWindowIcon(window, 1, &img);

    stbi_image_free(img.pixels);
}

}

#endif
