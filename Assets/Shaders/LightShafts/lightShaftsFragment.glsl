#version 330 core

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform sampler2D lightSourceMap;

uniform int lightShaftsIterations;
uniform float lightShaftsDensity;
uniform float lightShaftsWeight;
uniform float lightShaftsDecay;
uniform float lightShaftsIntensity;

#include "deferred.glsl"

vec3 CalcLightShafts (const in vec2 texCoord)
{
	vec3 lightShaft = vec3 (0.0);

	vec3 lightPosition = -lightDirection;
	vec4 lightPositionTexcoord = projectionMatrix * vec4 (lightPosition, 1.0);
	lightPositionTexcoord.xyz = lightPositionTexcoord.xyz * 0.5 + 0.5;
	lightPositionTexcoord.xyz /= lightPositionTexcoord.w;

	vec2 rayDir = texCoord - lightPositionTexcoord.xy;
	vec2 step = rayDir / float (lightShaftsIterations) * lightShaftsDensity;
	// deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
	float illuminationDecay = 1.0;

	// return vec3 (rayDir, 0);

	vec2 currentPos = texCoord;

	for (int i = 0; i < lightShaftsIterations; i++) {
		currentPos -= step;
		vec3 sample = texture (lightSourceMap, currentPos).xyz;
		sample *= illuminationDecay * lightShaftsWeight;

		lightShaft += sample;

		illuminationDecay *= lightShaftsDecay;
	}

	return lightShaft * lightShaftsIntensity;
}

void main ()
{
	vec2 texCoord = CalcTexCoord();

	out_color = CalcLightShafts (texCoord);
}
