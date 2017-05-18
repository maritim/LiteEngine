#version 330 core

layout(location = 0) out float out_shadowMap;

void main()
{
	/*
	 * OpenGL does this by default for DepthAttachment
	*/

	out_shadowMap = gl_FragCoord.z;
}
