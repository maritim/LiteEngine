#version 330 core

layout(location = 0) out float out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

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

uniform vec2 screenSize;

uniform vec2 ssdoShadowResolution;
uniform int ssdoShadowStride;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ssdoShadowResolution;
}

#include "ScreenSpace/screenSpaceRayTracing.glsl"

float CalcShadow (vec3 in_position)
{
	mat4 pixelProjectionMatrix = mat4 (
		0.5f * screenSize.x, 0, 0, 0,
		0, 0.5f * screenSize.y, 0, 0,
		0, 0, 0, 0,
		0.5f * screenSize.x, 0.5f * screenSize.y, 0, 1.0f) * projectionMatrix;

	vec2 reflectionPos;
	vec3 reflectionViewPos;

	bool intersect = traceScreenSpaceRay (in_position, -lightDirection, pixelProjectionMatrix,
		gPositionMap, screenSize, 1, -cameraZLimits.x, ssdoShadowStride, 5, 500,
		100.0f, reflectionPos, reflectionViewPos);

	return intersect == false ? 1.0 : 0.0;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture (gPositionMap, texCoord).xyz;

	out_color = CalcShadow (in_position);
}
