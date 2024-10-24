#version 450 core

layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aVertexTexCoord;

layout (location=0) out vec2 vTexCoord;

uniform mat4 uModel_to_NDC;
uniform float uCoordZ;

void main() {
	gl_Position = uModel_to_NDC * vec4(aVertexPosition, uCoordZ, 1.0);
	vTexCoord = aVertexTexCoord;
}