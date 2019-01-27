#version 330

in vec2 texcoord;
in vec4 color;

uniform sampler2D font;

layout (location = 0) out vec4 out_color;

void main ()
{
	out_color = color * texture (font, texcoord.st);
}
