#version 330

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;
uniform mat3 inverseNormalWorldMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform int volLightingIterations;
uniform float volLightingScattering;
uniform float volLightingIntensity;

#include "deferred.glsl"
#include "ShadowMap/cascadedShadowMap.glsl"

#define PI 3.14f

// Mie scaterring approximated with Henyey-Greenstein phase function.
float ComputeScattering(float lightDotView)
{
	float result = 1.0f - volLightingScattering * volLightingScattering;
	result /= (4.0f * PI * pow(1.0f + volLightingScattering * volLightingScattering - (2.0f * volLightingScattering) *      lightDotView, 1.5f));
	return result;
}

vec3 CalcVolumetricLighting (const in vec3 in_position)
{
	vec3 rayDir = in_position - cameraZLimits.x;

	float raySize =  length(rayDir);

	rayDir /= raySize;

	float stepSize = raySize / volLightingIterations;

	vec3 step = rayDir * stepSize;

	vec3 currentPos = rayDir * cameraZLimits.x + step;

	vec3 volLighting = vec3 (0.0);

	for (int i = 0; i < volLightingIterations; i++) {

		float shadow = CalcDirectionalShadowContributionDirect (currentPos);

		if (shadow > 0.0) {
			// return vec3 (1, 0, 0);
			volLighting += vec3 (ComputeScattering (dot (rayDir, lightDirection))) * lightColor;
		}

		currentPos += step;
	}

	volLighting /= volLightingIterations;

	return volLighting * volLightingIntensity;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;

	out_color = vec3 (0.0);

	if (in_position.z > -cameraZLimits.y) {
		out_color = CalcVolumetricLighting (in_position);
	}
}
