#version 450 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aVertexTexCoord;

layout (location=0) out vec2 vTexCoord;

uniform mat3 uModel_to_NDC;

void main() {
	gl_Position = vec4(vec2(uModel_to_NDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);
	vTexCoord = aVertexTexCoord;
}