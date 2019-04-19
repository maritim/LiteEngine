#version 330 core

layout(location = 0) out float out_shadowMap;

uniform float exponential;

void main()
{
	float depth = gl_FragCoord.z;

	depth = exp (exponential * depth);

	out_shadowMap = depth;
}
