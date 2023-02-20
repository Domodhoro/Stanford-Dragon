#version 300 es

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out highp vec3 FragNormal;
out highp vec3 FragPos;

void main() {
    gl_Position = Projection * View * Model * vec4(Position, 1.0f);
    FragNormal  = mat3(transpose(inverse(Model))) * Normal;
	FragPos     = vec3(Model * vec4(Position, 1.0f));
}
