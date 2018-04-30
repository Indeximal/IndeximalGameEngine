#version 400 core

in vec3 position;
in vec3 normal;
in vec2 uv_coord;

out vec3 frag_position;
flat out vec3 frag_normal;
out vec2 frag_uv;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
	vec4 worldPos = modelMat * vec4(position, 1.0);
	frag_position = worldPos.xyz;
	frag_normal = mat3(modelMat) * normal;
	frag_uv = uv_coord;
	gl_Position = projMat * viewMat * worldPos;
    //gl_Position = vec4(worldPos.xy - vec2(1.0), 0.0, 1.0);
}

