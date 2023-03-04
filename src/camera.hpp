#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace camera {

struct camera {
    camera(const int width, const int height) : aspect {static_cast<float>(width) / static_cast<float>(height)} {}

    void disable_cursor(GLFWwindow *window) const { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

    void set_position  (const glm::tvec3<float> Position) { this->Position = Position; }
    void set_aspect    (const float aspect)               { this->aspect   = aspect; }
    void set_FOV       (const float FOV)                  { this->FOV      = FOV; }
    void set_near_plane(const float near)                 { this->near     = near; }
    void set_far_plane (const float far)                  { this->far      = far; }

    glm::mat4 get_projection_matrix() const { return glm::perspective<float>(glm::radians(FOV), aspect, near, far); }
    glm::mat4 get_view_matrix      () const { return glm::lookAt<float>     (Position, Position + Front, Up); }

protected:
    float aspect {1.0f};
    float FOV    {60.0f};
    float near   {0.001f};
    float far    {1000.0f};

    glm::tvec3<float> Front    {0.0f, 0.0f, 1.0f};
    glm::tvec3<float> Position {0.0f};
    glm::tvec3<float> Up       {0.0f, 1.0f, 0.0f};
};

}

#endif
