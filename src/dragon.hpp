#ifndef DRAGON_HPP
#define DRAGON_HPP

#include "./mesh.hpp"
#include "./shader.hpp"

struct Dragon {
    Dragon() : m_shader("./glsl/dragonVertex.glsl", "./glsl/dragonFragment.glsl"), m_mesh("./dragon.obj") {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        unsigned int vertexBuffer = 0u, normalBuffer = 0u;

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_mesh.get_vertex_coords().size() * sizeof(glm::vec3), &m_mesh.get_vertex_coords().at(0), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)(0 * sizeof(float)));

        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_mesh.get_normal_coords().size() * sizeof(glm::vec3), &m_mesh.get_normal_coords().at(0), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)(0 * sizeof(float)));

        if (!m_VAO) my_exception {__FILE__, __LINE__, "falha ao criar VAO"};
    }

    ~Dragon() {
        glDeleteVertexArrays(1, &m_VAO);
    }

    void render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition) const {
        glCullFace(GL_FRONT);

        m_shader.use();

        glBindVertexArray(m_VAO);

        auto modelMatrix  {glm::mat4(1.0f)};
        auto compensation {glm::mat4(1.0f)};

        modelMatrix  = rotate(modelMatrix, glm::radians(static_cast<float>(glfwGetTime() * 30.0)), {0.0f, 1.0f, 0.0f});
        compensation = rotate(compensation, glm::radians(static_cast<float>(glfwGetTime() * -1.0 * m_rotationSpeed)), {0.0f, 1.0f, 0.0f});

        auto color         {glm::vec3(0.3f, 0.8f, 0.4f)};
        auto lightPosition {glm::vec3(-3.0f, 4.0f, -5.0f)};

        lightPosition = compensation * glm::vec4(lightPosition, 1.0f);

        const auto ambientLight     {0.0f};
        const auto specularStrength {10.0f};
        const auto shininess        {32.0f};

        m_shader.set_mat4 ("model", modelMatrix);
        m_shader.set_mat4 ("view", viewMatrix);
        m_shader.set_mat4 ("projection", projectionMatrix);
        m_shader.set_vec3 ("color", color);
        m_shader.set_vec3 ("cameraPosition", cameraPosition);
        m_shader.set_vec3 ("lightPosition", lightPosition);
        m_shader.set_float("ambientLight", ambientLight);
        m_shader.set_float("specularStrength", specularStrength);
        m_shader.set_float("shininess", shininess);

        glDrawArrays(GL_TRIANGLES, 0, m_mesh.getCount());

        glBindVertexArray(0);
    }

protected:
    unsigned int m_VAO          {0u};
    const float m_rotationSpeed {30.0f};

    shader::shader_program m_shader;
    mesh::mesh             m_mesh;
};

#endif
