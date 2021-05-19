#include "ShadowMap/shadowMap.glsl"

uniform sampler2D rsmPositionMap;
uniform sampler2D rsmNormalMap;
uniform sampler2D rsmFluxMap;

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

layout(std140) uniform rsmSamples
{
	int rsmSamplesCount;
	vec2 rsmSample[2000];
};

uniform float rsmRadius;

/*
 * Indirect Illumination Calculation
 * Thanks to: http://ericpolman.com/reflective-shadow-maps-part-2-the-implementation/
*/

vec3 CalcViewIndirectDiffuseLight (in vec3 viewPosition, in vec3 viewNormal)
{
	// Compute fragment world position and world normal
	vec3 lightViewSpacePos = vec3 (lightViewMatrix * inverseViewMatrix * vec4 (viewPosition, 1.0));
	vec3 lightViewSpaceNormal = vec3 (lightViewMatrix * vec4 (normalMatrix * inverseNormalWorldMatrix * viewNormal, 0.0));

	vec3 indirectColor = vec3 (0.0);

	vec4 lightSpacePos = lightProjectionMatrix * vec4 (lightViewSpacePos, 1.0);
	vec3 rsmProjCoords = (lightSpacePos.xyz / lightSpacePos.w) * 0.5 + 0.5;

	for (int index = 0; index < rsmSamplesCount; index ++) {

		vec2 rnd = rsmSample [index].xy;

		vec2 coords = rsmProjCoords.xy + rnd * rsmRadius;

		vec3 rsmLightViewSpacePos = texture2D (rsmPositionMap, coords).xyz;
		vec3 rsmLightViewSpaceNormal = texture2D (rsmNormalMap, coords).xyz;
		vec3 rsmFlux = texture2D (rsmFluxMap, coords).xyz;

		vec3 result = rsmFlux *
			((max (0.0, dot (rsmLightViewSpaceNormal, lightViewSpacePos - rsmLightViewSpacePos))
				* max (0.0, dot (lightViewSpaceNormal, rsmLightViewSpacePos - lightViewSpacePos)))
			/ pow (length (lightViewSpacePos - rsmLightViewSpacePos), 4.0));

		float dist = length (rnd);

		result = result * dist * dist;

		indirectColor += result;
	}

	indirectColor /= rsmSamplesCount;

	return indirectColor;
}
