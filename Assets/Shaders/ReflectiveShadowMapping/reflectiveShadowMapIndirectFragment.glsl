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

layout(std140) uniform rsmSamples
{
	int rsmSamplesCount;
	vec2 rsmSample[2000];
};

uniform vec2 rsmResolution;
uniform float rsmRadius;
uniform float rsmIntensity;
uniform int rsmNoiseEnabled;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;	
}

/*
 * Indirect Illumination Calculation
 * Thanks to: http://ericpolman.com/reflective-shadow-maps-part-2-the-implementation/
*/

vec3 CalcIndirectDiffuseLight (vec3 in_position, vec3 in_normal)
{
	// Compute fragment world position and world normal
	vec3 worldSpacePos = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldSpaceNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 indirectColor = vec3 (0.0);

	vec4 lightSpacePos = lightSpaceMatrix * vec4 (worldSpacePos, 1.0);
	vec3 rsmProjCoords = lightSpacePos.xyz / lightSpacePos.w;

	// vec2 texCoord = CalcTexCoordRSM();

	vec2 noiseTexcoord = worldSpacePos.xy + worldSpacePos.yz + worldSpacePos.xz;

	float r = 2 * 3.14 * rand (noiseTexcoord);
	vec2 randomVec = vec2 (cos (r), sin (r));
	mat2 tangentMatrix = mat2 (randomVec, vec2 (-randomVec.y, randomVec.x));

	// return vec3 (randomVec, 0);

	for (int index = 0; index < rsmSamplesCount; index ++) {

		vec2 rnd = rsmSample [index].xy;

		if (rsmNoiseEnabled == 1) {
			rnd = tangentMatrix * rnd;
		}

		vec2 coords = rsmProjCoords.xy + rnd * rsmRadius;

		vec3 rsmWorldSpacePos = texture2D (rsmPositionMap, coords).xyz;
		vec3 rsmWorldSpaceNormal = texture2D (rsmNormalMap, coords).xyz;
		vec3 rsmFlux = texture2D (rsmFluxMap, coords).xyz;

		vec3 result = rsmFlux *
			((max (0.0, dot (rsmWorldSpaceNormal, worldSpacePos - rsmWorldSpacePos))
				* max (0.0, dot (worldSpaceNormal, rsmWorldSpacePos - worldSpacePos)))
			/ pow (length (worldSpacePos - rsmWorldSpacePos), 4.0));

		float dist = length (rnd);

		result = result * dist * dist;

		indirectColor += result;
	}

	return clamp (indirectColor * rsmIntensity, 0.0, 1.0);
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcIndirectDiffuseLight(in_position, in_normal);
} 