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

uniform vec2 screenSize;

uniform vec2 ssrResolution;
uniform int ssrIterations;
uniform float ssrRoughness;
uniform float ssrSampleSkip;
uniform float ssrSpatialBias;

uniform sampler2D postProcessMap;

#include "ScreenSpace/screenSpaceRayTracing.glsl"

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ssrResolution;
}

// vec3 CalcScreenSpaceReflection (vec3 in_position, vec3 in_normal, vec2 texCoord)
// {
// 	vec3 viewDirection = normalize (in_position);

// 	vec3 reflectionDirection = normalize (reflect (viewDirection, in_normal));

// 	float radius = 100.0f;
// 	vec3 reflectionFinalPos = in_position + reflectionDirection * radius;
// 	vec2 projectedFinalPos = projectionMatrix * vec4 (reflectionFinalPos, 1.0f);
// 	projectedFinalPos.xyz /= projectedPos.w;
// 	projectedFinalPos.xyz = projectedFinalPos.xyz * 0.5f + 0.5f;

// 	vec2 reflectionDir = projectedFinalPos - texCoord;

// 	int iterations = 0;

// 	bool needStop = false;

// 	float dx = (projectedFinalPos.x - texCoord.x);
// 	float dy = (projectedFinalPos.y - texCoord.y);

// 	float step = abs (dx) >= abs (dy) ? abs (dx) : abs (dy);

// 	dx /= step; dy /= step;

// 	vec2 projectedPos = texCoord;

// 	while (iterations < ssrIterations && needStop == false) {

// 		vec3 samplePos = texture (gPositionMap, projectedPos).xyz;

// 		if (rayPos.z < samplePos.z) {
// 			needStop = true;

// 			if (rayPos.z > samplePos.z - ssrSpatialBias) {
// 				reflectionPos = projectedPos.xyz;
// 			}
// 		}

// 		projectedPos += vec2 (dx, dy);

// 		iterations ++;
// 	}
// }

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

	vec2 reflectionPos = ScreenSpaceRayTrace (in_position, reflectionDirection, gPositionMap,
		ssrIterations, ssrSampleSkip, ssrSpatialBias);

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

	if (length (in_specular) > 0.0f) {
		reflectionPos = CalcScreenSpaceReflection (in_position, in_normal, texCoord);
	}

	out_color = reflectionPos;
}
