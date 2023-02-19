#ifndef DRAGON_HPP
#define DRAGON_HPP

#include "./mesh.hpp"
#include "./shader.hpp"

namespace dragon {

struct dragon {
    dragon() : m_mesh("./dragon.obj") {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_mesh.get_vertice().size() * 6 * sizeof(float), &m_mesh.get_vertice().at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO"};

        glBindVertexArray(0);
    }

    ~dragon() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
    }

    void render(const shader::shader_program &shader, const camera::camera &cam) const {
        glCullFace(GL_FRONT);

        shader.use();

        glBindVertexArray(m_VAO);

        auto model {glm::mat4(1.0f)};

        model = rotate(model, glm::radians<float>(glfwGetTime() * 30.0), {0.0f, 1.0f, 0.0f});

        auto light_color    {glm::vec3(0.3f, 0.8f, 0.4f)};
        auto light_position {glm::vec3(-3.0f, 4.0f, -5.0f)};

        shader.set_mat4("Model", model);
        shader.set_mat4("View", cam.get_view_matrix());
        shader.set_mat4("Projection", cam.get_projection_matrix());
        shader.set_vec3("light_color", light_color);
        shader.set_vec3("light_position", light_position);

        glDrawArrays(GL_TRIANGLES, 0, m_mesh.get_indices().size());

        glBindVertexArray(0);
    }

protected:
    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};

    const float m_rotationSpeed {30.0f};

    mesh::mesh m_mesh;
};

}

#endif
