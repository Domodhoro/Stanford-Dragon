#ifndef MESH_HPP
#define MESH_HPP

struct vertex {
    glm::tvec2<float> position       {0.0f, 0.0f};
    glm::tvec2<float> texture_coords {0.0f, 0.0f};
};

class Mesh {
public:
    Mesh(std::vector<vertex> &vertices, std::vector<unsigned int> &indices);

    virtual ~Mesh();

    void set_texture(const unsigned int &texture);
    void set_shader (const unsigned int &shader);

    void draw(const glm::mat4 &projection);

    void set_translation(const glm::tvec2<float> translation);
    void set_rotation   (const float rotation);
    void set_scale      (const float scale);

private:
    unsigned int texture {0u};
    unsigned int shader  {0u};

    unsigned int VAO {0u};
    unsigned int VBO {0u};
    unsigned int EBO {0u};

    int count {0};

    glm::tvec2<float> translation {0.0f};

    float rotation {0.0f};
    float scale    {1.0f};
};

Mesh::Mesh(std::vector<vertex> &vertices, std::vector<unsigned int> &indices) : count {indices.size()} {
    glGenVertexArrays(1, &VAO);
    glGenBuffers     (1, &VBO);
    glGenBuffers     (1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices.at(0), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.at(0), GL_STATIC_DRAW);

    glVertexAttribPointer    (0, 2, GL_FLOAT, false, sizeof(vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer    (1, 2, GL_FLOAT, false, sizeof(vertex), (void *)offsetof(vertex, texture_coords));
    glEnableVertexAttribArray(1);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers     (1, &VBO);
    glDeleteBuffers     (1, &EBO);
    glDeleteTextures    (1, &texture);
    glDeleteProgram     (shader);
}

void Mesh::set_texture(const unsigned int &texture) { this->texture = texture; }

void Mesh::set_shader(const unsigned int &shader) { this->shader = shader; }

void Mesh::draw(const glm::mat4 &projection) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram (shader);

    glm::mat4 model {1.0f};

    model = glm::translate(model, glm::tvec3<float>(translation, 0.0f));
    model = glm::rotate   (model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale    (model, glm::tvec3<float>(scale));

    glUniform1f       (glGetUniformLocation(shader, "Time"), static_cast<float>(glfwGetTime()));
    glUniformMatrix4fv(glGetUniformLocation(shader, "Model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader, "Projection"), 1, false, glm::value_ptr(projection));
    glBindVertexArray (VAO);
    glDrawElements    (GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)0);
}

void Mesh::set_translation(const glm::tvec2<float> translation) { this->translation = translation; }

void Mesh::set_rotation(const float rotation) { this->rotation = rotation; }

void Mesh::set_scale(const float scale) { this->scale = scale; }

#endif
