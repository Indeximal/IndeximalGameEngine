#version 400 core

in vec3 position;

out vec3 color;

void main() {
	gl_Position = vec4(position, 1.0);

	color = vec3(position) + vec3(0.5);
}
