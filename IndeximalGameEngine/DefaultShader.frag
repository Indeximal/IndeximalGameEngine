#version 400 core

in vec3 color;

out vec4 out_Color;

uniform vec3 tintColor;

void main() {

	out_Color = vec4(mix(color, tintColor, 0.5), 1.0);
}