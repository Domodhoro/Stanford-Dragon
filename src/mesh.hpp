#ifndef MESH_HPP
#define MESH_HPP

namespace mesh {

struct mesh {
    mesh(const char *meshPath) : file {fopen(meshPath, "r")} {
        if (file == nullptr) my_exception {__FILE__, __LINE__, "falha ao abrir arquivo 'dragon.obj'"};

        while (true) {
            char line[128];

            if (fscanf(file, "%s", line) == EOF) break;

            if (strcmp(line, "v") == 0) {
                vec3<float> vertex {0.0f, 0.0f, 0.0f};

                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

                temp_vertex.emplace_back(vertex);
            } else if (strcmp(line, "vn") == 0) {
                vec3<float> normal {0.0f, 0.0f, 0.0f};

                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);

                temp_normal.emplace_back(normal);
            } else if (strcmp(line, "f") == 0) {
                unsigned int index[6] = {0u, 0u, 0u, 0u, 0u, 0u};

                fscanf(file, "%d//%d %d//%d %d//%d\n", &index[0], &index[3], &index[1], &index[4], &index[2], &index[5]);

                indice.emplace_back(index[2]);
                indice.emplace_back(index[1]);
                indice.emplace_back(index[0]);
                indice.emplace_back(index[5]);
                indice.emplace_back(index[4]);
                indice.emplace_back(index[3]);
            }
        }

        for(auto i = 0; i != static_cast<int>(indice.size()); ++i) {
            const vec3_n<float> vertex {
                temp_vertex.at(indice.at(i) - 1).x,
                temp_vertex.at(indice.at(i) - 1).y,
                temp_vertex.at(indice.at(i) - 1).z,
                temp_normal.at(indice.at(i) - 1).x,
                temp_normal.at(indice.at(i) - 1).y,
                temp_normal.at(indice.at(i) - 1).z
            };

            vertice.emplace_back(vertex);
        }
    }

    ~mesh() {
        fclose(file);

        temp_vertex.clear();
        temp_normal.clear();
    }

    std::vector<vec3_n<float>> get_vertice() const { return vertice; }
    std::vector<unsigned int> get_indice  () const { return indice; }

protected:
    FILE *file {nullptr};

    std::vector<vec3<float>>   temp_vertex;
    std::vector<vec3<float>>   temp_normal;
    std::vector<vec3_n<float>> vertice;
    std::vector<unsigned int>  indice;
};

}

#endif
