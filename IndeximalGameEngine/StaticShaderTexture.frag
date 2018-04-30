#version 400 core

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;

out vec4 out_Color;

uniform vec3 lightDirection;
uniform vec3 viewPosition;
uniform vec3 tint;
uniform sampler2D tex;

const float ambientConst = 0.2;
const float diffuseConst = 0.8;
const float shininessConst = 16;
const vec3 specularColor = vec3(0.5);

void main() {
    vec4 texColor = texture(tex, frag_uv);
    if (texColor.a < 0.1) discard;
    vec3 color = texColor.rgb + tint;

	vec3 N = normalize(frag_normal);
	vec3 L = -normalize(lightDirection);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(viewPosition - frag_position);

	vec3 ambient = color * ambientConst;
	vec3 diffuse = color * diffuseConst * max(dot(L, N), 0.0);
	vec3 specular = specularColor * pow(max(dot(R, V), 0.0), shininessConst);

	out_Color = vec4(ambient + diffuse + specular, 1.0);
}