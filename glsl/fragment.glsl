#version 330 core

out vec4 FragColor;
in  vec2 TexCoord;

uniform sampler2D Sampler;
uniform float     Time;

vec2 tex_sample(vec2 tex, float size, float x, float y);

void main() {
    vec4  color = vec4(1.0f);
    float size  = 0.25f;
    float frame = mod(8.0f * Time / 4.0f, 1.0f);

    if      (frame < 0.25) color = texture(Sampler, tex_sample(TexCoord, size, 0.0f, 0.0f));
    else if (frame < 0.50) color = texture(Sampler, tex_sample(TexCoord, size, 1.0f, 0.0f));
    else if (frame < 0.75) color = texture(Sampler, tex_sample(TexCoord, size, 2.0f, 0.0f));
    else                   color = texture(Sampler, tex_sample(TexCoord, size, 3.0f, 0.0f));

    if (color.r == 1.0f && color.g != 1.0f && color.b == 1.0f) discard;

    FragColor = color;
}

vec2 tex_sample(vec2 tex, float size, float x, float y) { return vec2((tex.x + x) * size, (tex.y + y) * size); }
