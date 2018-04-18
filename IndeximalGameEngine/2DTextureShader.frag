#version 400 core

in vec2 frag_TexCoords;

out vec4 out_Color;

uniform sampler2D tex;

void main() {
    out_Color = texture(tex, frag_TexCoords);
}
