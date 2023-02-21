#ifndef DRAGON_HPP
#define DRAGON_HPP

#include "./mesh.hpp"
#include "./shader.hpp"

namespace dragon {

struct dragon {
    dragon() : m_mesh("./dragon.obj") { mesh_setup(); }

    ~dragon() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers     (1, &m_VBO);
    }

    void set_light_position(const glm::tvec3<float> light_position) { m_light_position = light_position; }
    void set_light_color   (const glm::tvec3<float> light_color)    { m_light_color = light_color; }

    void render(const shader::shader_program &shader, const camera::camera &cam, const glm::mat4 &model) const {
        glCullFace(GL_FRONT);

        shader.use();

        shader.set_mat4("Model", model);
        shader.set_mat4("View", cam.get_view_matrix());
        shader.set_mat4("Projection", cam.get_projection_matrix());
        shader.set_vec3("light_position", m_light_position);
        shader.set_vec3("light_color", m_light_color);

        glBindVertexArray(m_VAO);
        glDrawArrays     (GL_TRIANGLES, 0, m_mesh.get_indices().size());
        glBindVertexArray(0);

        glCullFace(GL_BACK);
    }

protected:
    unsigned int m_VAO {0u};
    unsigned int m_VBO {0u};

    glm::tvec3<float> m_light_position {0.0f};
    glm::tvec3<float> m_light_color    {0.0f};

    mesh::mesh m_mesh;

    void mesh_setup() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers     (1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_mesh.get_vertice().size() * sizeof(vec3_n<float>), &m_mesh.get_vertice().at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, sizeof(vec3_n<float>), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 3, GL_FLOAT, false, sizeof(vec3_n<float>), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (m_VAO == 0u) my_exception {__FILE__, __LINE__, "falha ao criar VAO"};

        glBindVertexArray(0);
    }
};

}

#endif
