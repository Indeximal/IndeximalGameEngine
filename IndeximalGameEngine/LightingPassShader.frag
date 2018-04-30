#version 400 core

const float ambientConst = 0.2;
const float diffuseConst = 0.8;
const float shininessConst = 16;
const vec3 specularColor = vec3(0.5);

in vec2 frag_TexCoords;

uniform vec3 lightDirection;
uniform vec3 viewPosition;

uniform sampler2D albedoBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D positionBuffer;

out vec4 out_Color;

void main() {
    vec3 color = texture(albedoBuffer, frag_TexCoords).xyz;
    vec3 normal = texture(normalBuffer, frag_TexCoords).xyz;
    vec3 position = texture(positionBuffer, frag_TexCoords).xyz;

    if (normal == vec3(0.0)) {
        out_Color = vec4(color, 1.0);
        return;
    }      

    vec3 N = normalize(normal);
	vec3 L = -normalize(lightDirection);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(viewPosition - position.xyz);

	vec3 ambient = color * ambientConst;
	vec3 diffuse = color * diffuseConst * max(dot(L, N), 0.0);
	vec3 specular = specularColor * pow(max(dot(R, V), 0.0), shininessConst);

	out_Color = vec4(ambient + diffuse + specular, 1.0);
    //out_Color = vec4(vec3(length(normal)), 1.0);
}