#ifndef MESH_HPP
#define MESH_HPP

namespace mesh {

struct mesh {
    mesh(const char *meshPath) : file(fopen(meshPath, "r")) {
        if (file == nullptr) my_exception {__FILE__, __LINE__, "falha ao abrir arquivo modelo"};

        while (true) {
            char line[128];

            if (fscanf(file, "%s", line) == EOF) break;

            if (strcmp(line, "v") == 0) {
                auto vertex {glm::tvec3<float>(0.0)};

                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

                m_temp_vertex.emplace_back(vertex);
            } else if (strcmp(line, "vn") == 0) {
                auto normal {glm::tvec3<float>(0.0)};

                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);

                m_temp_normal.emplace_back(normal);
            } else if (strcmp(line, "f") == 0) {
                unsigned int vertex_index[3] = {0u, 0u, 0u};
                unsigned int normal_index[3] = {0u, 0u, 0u};

                fscanf(file, "%d//%d %d//%d %d//%d\n", &vertex_index[0], &normal_index[0], &vertex_index[1], &normal_index[1], &vertex_index[2], &normal_index[2]);

                m_vertex_indices.emplace_back(vertex_index[2]);
                m_vertex_indices.emplace_back(vertex_index[1]);
                m_vertex_indices.emplace_back(vertex_index[0]);

                m_normal_indices.emplace_back(normal_index[2]);
                m_normal_indices.emplace_back(normal_index[1]);
                m_normal_indices.emplace_back(normal_index[0]);
            }
        }

        setup();
    }

    ~mesh() {
        fclose(file);

        m_temp_vertex.clear();
        m_temp_normal.clear();
        m_vertex_indices.clear();
        m_normal_indices.clear();
    }

    std::vector<glm::tvec3<float>> get_vertex_coords() const { return m_vertex_coords; }
    std::vector<glm::tvec3<float>> get_normal_coords() const { return m_normal_coords; }
    unsigned int getCount()                            const { return m_count; }

protected:
    FILE *file {nullptr};

    std::vector<glm::tvec3<float>> m_vertex_coords;
    std::vector<glm::tvec3<float>> m_normal_coords;
    std::vector<glm::tvec3<float>> m_temp_vertex;
    std::vector<glm::tvec3<float>> m_temp_normal;
    std::vector<unsigned int>      m_vertex_indices;
    std::vector<unsigned int>      m_normal_indices;

    unsigned int m_count {0u};

    void setup() {
        for(auto i = 0; i != static_cast<int>(m_vertex_indices.size()); ++i) {
            auto vertex {m_temp_vertex.at(m_vertex_indices.at(i) - 1)};
            auto normal {m_temp_normal.at(m_normal_indices.at(i) - 1)};

            m_vertex_coords.emplace_back(vertex);
            m_normal_coords.emplace_back(normal);

            ++m_count;
        }
    }
};

}

#endif
