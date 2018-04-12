#version 400 core

in vec3 model_Pos;
in vec3 frag_normal;

out vec4 out_Color;

uniform vec3 tintColor;
uniform vec3 sunDirection;


void main() {
	vec3 normal = normalize(frag_normal);
	out_Color = vec4(tintColor * dot(normal, sunDirection), 1.0);
}