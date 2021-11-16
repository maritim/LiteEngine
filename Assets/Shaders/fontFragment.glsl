#version 330 core

layout(location = 0) out vec4 out_color;

uniform sampler2D FontTexture;

in vec2 texcoord;

void main()
{
	vec4 color = texture (FontTexture, texcoord);

	out_color = color;
}
