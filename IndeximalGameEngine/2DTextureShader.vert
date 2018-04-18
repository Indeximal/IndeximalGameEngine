#version 400 core

in vec2 position;
in vec2 texCoords;

out vec2 frag_TexCoords;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    frag_TexCoords = texCoords;
}
