#version 330 core

layout(location = 0) out float out_depth;

void main()
{
	/*
	 * OpenGL does this by default for DepthAttachment
	*/

	out_depth = gl_FragCoord.z;
}
