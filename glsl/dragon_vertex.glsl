#version 300 es

layout(location = 0) in vec3 vertex_coords;
layout(location = 1) in vec3 normal_coords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out highp vec3 Normal;
out highp vec3 FragPos;

void main() {
    gl_Position = Projection * View * Model * vec4(vertex_coords, 1.0f);
    Normal      = mat3(transpose(inverse(Model))) * normal_coords;
	FragPos     = vec3(Model * vec4(vertex_coords, 1.0f));
}
