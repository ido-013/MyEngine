#version 450 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uTex2d;
uniform vec4 uColor;

void main() {
	vec4 texColor = texture(uTex2d, vTexCoord);

	if (texColor.a == 0)
	{
		discard;
	}

	fFragColor = texColor * uColor;
}