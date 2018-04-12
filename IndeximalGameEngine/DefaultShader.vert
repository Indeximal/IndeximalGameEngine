#version 400 core

in vec3 position;
in vec3 normal;

out vec3 model_Pos;
out vec3 frag_normal;

uniform mat4 MVPMatrix;

void main() {
	gl_Position = MVPMatrix * vec4(position, 1.0);
	model_Pos = position;
	frag_normal = normal;
}

