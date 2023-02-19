#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace camera {

struct camera {
    camera() = default;

    camera(const int width, const int height) : m_aspect {static_cast<float>(width) / static_cast<float>(height)} {}

    void disable_cursor(GLFWwindow *window) const { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

    void set_position   (const glm::tvec3<float> Position) { m_Position    = Position; }
    void set_aspect     (const float aspect)               { m_aspect      = aspect; }
    void set_FOV        (const float FOV)                  { m_FOV         = FOV; }
    void set_near_plane (const float near)                 { m_near        = near; }
    void set_far_plane  (const float far)                  { m_far         = far; }

    glm::tvec3<float> get_position () const { return m_Position; }
    glm::mat4 get_projection_matrix() const { return glm::perspective<float>(glm::radians(m_FOV), m_aspect, m_near, m_far); }
    glm::mat4 get_view_matrix      () const { return glm::lookAt<float>     (m_Position, m_Position + m_Front, m_Up); }

protected:
    float m_aspect      {1.0f};
    float m_FOV         {60.0f};
    float m_near        {0.001f};
    float m_far         {1000.0f};

    glm::tvec3<float> m_Front     {0.0f, 0.0f, 1.0f};
    glm::tvec3<float> m_Position  {0.0f};
    glm::tvec3<float> m_Up        {0.0f, 1.0f, 0.0f};
};

}

#endif
