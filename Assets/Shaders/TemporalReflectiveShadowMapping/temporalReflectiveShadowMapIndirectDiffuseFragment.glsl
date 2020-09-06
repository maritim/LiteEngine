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

uniform vec2 rsmResolution;
uniform float rsmIntensity;

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
	vec3 lightViewSpacePos = vec3 (lightViewMatrix * inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 lightViewSpaceNormal = vec3 (lightViewMatrix * vec4 (normalMatrix * inverseNormalWorldMatrix * in_normal, 0.0));

	vec3 indirectColor = vec3 (0.0);

	vec4 lightSpacePos = lightProjectionMatrix * vec4 (lightViewSpacePos, 1.0);
	vec3 rsmProjCoords = (lightSpacePos.xyz / lightSpacePos.w) * 0.5 + 0.5;

	vec2 noiseTexcoord = lightViewSpacePos.xy + lightViewSpacePos.yz + lightViewSpacePos.xz;

	float r = 2 * 3.14 * rand (noiseTexcoord);
	vec2 randomVec = vec2 (cos (r), sin (r));
	mat2 tangentMatrix = mat2 (randomVec, vec2 (-randomVec.y, randomVec.x));

	for (int index = 0; index < rsmSamplesCount; index ++) {

		vec2 rnd = tangentMatrix * rsmSample [index].xy;

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

	return clamp (indirectColor * rsmIntensity, 0.0, 1.0);
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcIndirectDiffuseLight(in_position, in_normal);
} 