#version 400 core

in vec3 position;

out vec3 color;

uniform mat4 MVPMatrix;

void main() {
	gl_Position = MVPMatrix * vec4(position, 1.0);

	color = vec3(position) + vec3(0.5);
}

