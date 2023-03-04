#ifndef DRAGON_HPP
#define DRAGON_HPP

namespace dragon {

struct dragon {
    dragon() : mesh("./dragon.obj") { mesh_setup(); }

    ~dragon() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers     (1, &VBO);
    }

    void set_light_position(const glm::tvec3<float> light_position) { this->light_position = light_position; }
    void set_light_color   (const glm::tvec3<float> light_color)    { this->light_color    = light_color; }

    void render(const glm::mat4 &model, const shader::shader_program &shader, const camera::camera &cam) const {
        glCullFace(GL_FRONT);

        shader.use();

        shader.set_mat4("Model", model);
        shader.set_mat4("View", cam.get_view_matrix());
        shader.set_mat4("Projection", cam.get_projection_matrix());
        shader.set_vec3("light_position", light_position);
        shader.set_vec3("light_color", light_color);

        glBindVertexArray(VAO);
        glDrawArrays     (GL_TRIANGLES, 0, mesh.get_indice().size());
        glBindVertexArray(0);

        glCullFace(GL_BACK);
    }

protected:
    unsigned int VAO {0u};
    unsigned int VBO {0u};

    glm::tvec3<float> light_position {0.0f};
    glm::tvec3<float> light_color    {0.0f};

    mesh::mesh mesh;

    void mesh_setup() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers     (1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.get_vertice().size() * sizeof(vertex<float>), &mesh.get_vertice().at(0), GL_STATIC_DRAW);

        glVertexAttribPointer    (0, 3, GL_FLOAT, false, sizeof(vertex<float>), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer    (1, 3, GL_FLOAT, false, sizeof(vertex<float>), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        if (VAO == 0u) error_log(__FILE__, __LINE__, "falha ao criar VAO");

        glBindVertexArray(0);
    }
};

}

#endif
