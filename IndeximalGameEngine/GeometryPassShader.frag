#version 400 core

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;

layout (location = 0) out vec3 out_Color;
layout (location = 1) out vec3 out_Normal;
layout (location = 2) out vec3 out_Position;

uniform sampler2D tex;

void main() {
    vec4 texColor = texture(tex, frag_uv);
    if (texColor.a < 0.1) discard;
    out_Color = texColor.rgb;
    out_Normal = frag_normal;
    out_Position = frag_position;
}