#version 330 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 Texture;

out vec2 TexCoord;

uniform mat4 Model;
uniform mat4 Projection;

void main() {
    gl_Position = Projection * Model * vec4(Position.xy, 0.0f, 1.0f);
    TexCoord    = Texture;
}
