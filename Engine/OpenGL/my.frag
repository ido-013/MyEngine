R"(#version 450 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uTex2d;
uniform vec4 uColor;

void main() {
	fFragColor = texture(uTex2d, vTexCoord) * uColor;
}
)"