#version 330 core

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;
uniform sampler2D gDepthMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform vec2 screenSize;

uniform vec2 ssrResolution;
uniform int ssrIterations;
uniform float ssrRoughness;
uniform float ssrThickness;
uniform int ssrStride;

uniform sampler2D postProcessMap;

#include "ScreenSpace/screenSpaceRayTracing.glsl"

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ssrResolution;
}

const vec3 HASHSCALE3 = vec3 (0.8f);

vec3 hash (vec3 a)
{
    a = fract (a * HASHSCALE3);
    a += dot (a, a.yxz + 19.19);
    return fract ((a.xxy + a.yxx)*a.zyx);
}

vec3 CalcScreenSpaceReflection (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	vec3 jitter = mix (vec3 (0.0f), hash (in_position), ssrRoughness);

	vec3 viewDirection = normalize (in_position);

	vec3 reflectionDirection = jitter + normalize (reflect (viewDirection, in_normal));

	mat4 pixelProjectionMatrix = mat4 (
		0.5f * screenSize.x, 0, 0, 0,
		0, 0.5f * screenSize.y, 0, 0,
		0, 0, 0, 0,
		0.5f * screenSize.x, 0.5f * screenSize.y, 0, 1.0f) * projectionMatrix;

	vec2 reflectionPos;
	vec3 reflectionViewPos;

	bool intersect = traceScreenSpaceRay (in_position, reflectionDirection, pixelProjectionMatrix,
		gPositionMap, screenSize, ssrThickness, -cameraZLimits.x, ssrStride, 2, ssrIterations,
		1000.0f, reflectionPos, reflectionViewPos);

	if (intersect == false) {
		return vec3 (0.0);
	}

	/*
	 * Discard reflections beyond camera z
	*/

	if (reflectionViewPos.z <= -cameraZLimits.y) {
		return vec3 (0.0);
	}

	reflectionPos /= screenSize.xy;

	return vec3 (reflectionPos, 0.0);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;
	vec3 in_position = texture (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture (gNormalMap, texCoord).xyz;
	vec3 in_specular = texture (gSpecularMap, texCoord).xyz;
	float in_depth = texture (gDepthMap, texCoord).x;

	in_normal = normalize(in_normal);

	vec3 reflectionPos = vec3 (0.0f);

	if (length (in_specular) > 0.0f && in_position.z >= -cameraZLimits.y) {
		reflectionPos = CalcScreenSpaceReflection (in_position, in_normal, texCoord);
	}

	out_color = reflectionPos;
}
