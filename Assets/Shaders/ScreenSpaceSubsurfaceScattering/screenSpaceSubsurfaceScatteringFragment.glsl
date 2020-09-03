#version 330 core

layout(location = 0) out vec3 out_color;

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

uniform vec2 ssrResolution;
uniform int ssrIterations;
uniform float ssrRoughness;
uniform float ssrThickness;
uniform int ssrStride;

uniform sampler2D postProcessMap;

#include "deferred.glsl"
#include "ScreenSpace/screenSpaceRayTracing.glsl"

const vec3 HASHSCALE3 = vec3 (0.8f);

vec3 hash (vec3 a)
{
    a = fract (a * HASHSCALE3);
    a += dot (a, a.yxz + 19.19);
    return fract ((a.xxy + a.yxx)*a.zyx);
}

vec2 CalcScreenSpaceSubsurfaceScattering (vec3 in_position, vec3 in_normal, float in_refractiveIndex)
{
	// vec3 jitter = mix (vec3 (0.0f), hash (in_position), ssrRoughness);

	vec3 viewDirection = normalize (in_position);
	vec3 refractiveDirection = refract (viewDirection, in_normal, 1.0 / in_refractiveIndex);

	mat4 pixelProjectionMatrix = mat4 (
		0.5f * screenSize.x, 0, 0, 0,
		0, 0.5f * screenSize.y, 0, 0,
		0, 0, 0, 0,
		0.5f * screenSize.x, 0.5f * screenSize.y, 0, 1.0f) * projectionMatrix;

	vec2 reflectionPos;
	vec3 reflectionViewPos;

	bool intersect = traceScreenSpaceRay (in_position, refractiveDirection, pixelProjectionMatrix,
		gPositionMap, screenSize, 5, -cameraZLimits.x, 1, 2, ssrIterations,
		1000.0f, reflectionPos, reflectionViewPos);

	if (intersect == false) {
		return vec2 (0.0);
	}

	return reflectionPos / screenSize;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;
	vec3 in_position = texture (gTrPositionMap, texCoord).xyz;
	vec3 in_normal = texture (gTrNormalMap, texCoord).xyz;
	float in_refractiveIndex = texture (gTrNormalMap, texCoord).w;
	float in_transparency = texture (gTrDiffuseMap, texCoord).w;

	in_normal = normalize(in_normal);

	vec2 reflectionPos = vec2 (0.0f);

	if (in_transparency < 1.0f) {
		reflectionPos = CalcScreenSpaceSubsurfaceScattering (in_position, in_normal, in_refractiveIndex);

		vec3 in_light = texture (postProcessMap, reflectionPos).xyz;

		out_color = mix (in_diffuse, in_light, in_transparency);
	} else {
		out_color = in_diffuse;
	}
}
