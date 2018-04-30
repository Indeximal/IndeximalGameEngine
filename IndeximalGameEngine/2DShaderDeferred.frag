#version 400 core

in vec2 frag_TexCoords;

layout (location = 0) out vec3 out_Color;
layout (location = 1) out vec3 out_Normal;
layout (location = 2) out vec3 out_Position;

uniform sampler2D tex;

void main() {
    vec4 color = texture(tex, frag_TexCoords);
    if (color.a < 0.1) discard;
    out_Color = color.rgb;
    out_Normal = vec3(0.0);
    out_Position = vec3(0.0);
}
