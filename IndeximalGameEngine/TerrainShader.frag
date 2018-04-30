#version 400 core

in vec3 frag_position;
flat in vec3 frag_normal;
in vec2 frag_uv;

out vec4 out_Color;

uniform vec3 lightDirection;
uniform vec3 viewPosition;
uniform vec3 color;

const float ambientConst = 0.3;
const float diffuseConst = 0.7;
const float shininessConst = 16;
const vec3 specularColor = vec3(0.2);

void main() {
	vec3 N = normalize(frag_normal);
	vec3 L = -normalize(lightDirection);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(viewPosition - frag_position);

	vec3 ambient = color * ambientConst;
	vec3 diffuse = color * diffuseConst * max(dot(L, N), 0.0);
	vec3 specular = specularColor * pow(max(dot(R, V), 0.0), shininessConst);

	out_Color = vec4(ambient + diffuse + specular, 1.0);
}