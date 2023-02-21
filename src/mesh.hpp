#ifndef MESH_HPP
#define MESH_HPP

namespace mesh {

struct mesh {
    mesh(const char *meshPath) : m_file(fopen(meshPath, "r")) {
        if (m_file == nullptr) my_exception {__FILE__, __LINE__, "falha ao abrir arquivo 'dragon.obj'"};

        while (true) {
            char line[128];

            if (fscanf(m_file, "%s", line) == EOF) break;

            if (strcmp(line, "v") == 0) {
                vec3<float> vertex {0.0f, 0.0f, 0.0f};

                fscanf(m_file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

                m_temp_vertex.emplace_back(vertex);
            } else if (strcmp(line, "vn") == 0) {
                vec3<float> normal {0.0f, 0.0f, 0.0f};

                fscanf(m_file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);

                m_temp_normal.emplace_back(normal);
            } else if (strcmp(line, "f") == 0) {
                unsigned int index[6] = {0u, 0u, 0u, 0u, 0u, 0u};

                fscanf(m_file, "%d//%d %d//%d %d//%d\n", &index[0], &index[3], &index[1], &index[4], &index[2], &index[5]);

                m_indices.emplace_back(index[2]);
                m_indices.emplace_back(index[1]);
                m_indices.emplace_back(index[0]);
                m_indices.emplace_back(index[5]);
                m_indices.emplace_back(index[4]);
                m_indices.emplace_back(index[3]);
            }
        }

        for(auto i = 0; i != static_cast<int>(m_indices.size()); ++i) {
            const vec3_n<float> vertex {
                m_temp_vertex.at(m_indices.at(i) - 1).x,
                m_temp_vertex.at(m_indices.at(i) - 1).y,
                m_temp_vertex.at(m_indices.at(i) - 1).z,
                m_temp_normal.at(m_indices.at(i) - 1).x,
                m_temp_normal.at(m_indices.at(i) - 1).y,
                m_temp_normal.at(m_indices.at(i) - 1).z
            };

            m_vertice.emplace_back(vertex);
        }
    }

    ~mesh() {
        fclose(m_file);

        m_temp_vertex.clear();
        m_temp_normal.clear();
    }

    std::vector<vec3_n<float>> get_vertice() const { return m_vertice; }
    std::vector<unsigned int> get_indices () const { return m_indices; }

protected:
    FILE *m_file {nullptr};

    std::vector<vec3<float>>   m_temp_vertex;
    std::vector<vec3<float>>   m_temp_normal;
    std::vector<vec3_n<float>> m_vertice;
    std::vector<unsigned int>  m_indices;
};

}

#endif
