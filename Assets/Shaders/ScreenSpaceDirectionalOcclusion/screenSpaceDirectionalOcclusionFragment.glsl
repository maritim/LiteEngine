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

uniform vec2 ssdoResolution;
uniform int ssdoInterpolationEnabled;
uniform float ssdoIndirectIntensity;
uniform float ssdoInterpolationScale;
uniform float ssdoMinInterpolationDistance;
uniform float ssdoMinInterpolationAngle;
uniform int ssdoDebugInterpolation;

uniform sampler2D ssdoMap;

vec2 CalcTexCoordSSDO()
{
	return gl_FragCoord.xy / ssdoResolution;
}

#include "deferred.glsl"
#include "ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusion.glsl"

vec3 CalcInterpolatedIndirectDiffuseLight (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	if (dot (in_position, in_position) == 0) {
		return vec3 (0);
	}

	vec3 indirectDiffuseLight = vec3 (0);

	vec3 interpolatedPosition = textureLod (gPositionMap, texCoord, log2 (1.0 / ssdoInterpolationScale)).xyz;
	vec3 interpolatedNormal = textureLod (gNormalMap, texCoord, log2 (1.0 / ssdoInterpolationScale)).xyz;

	if (distance (interpolatedPosition, in_position) < ssdoMinInterpolationDistance
		&& dot (interpolatedNormal, in_normal) > ssdoMinInterpolationAngle) {
		indirectDiffuseLight = texture (ssdoMap, texCoord).xyz;
	} else {
		if (ssdoDebugInterpolation == 1) {
			indirectDiffuseLight = vec3 (1, 0, 0);
		} else {
			indirectDiffuseLight = CalcScreenSpaceDirectionalOcclusion(in_position, in_normal) * ssdoIndirectIntensity;
		}
	}

	return indirectDiffuseLight;
}

void main()
{
	vec2 texCoord = CalcTexCoordSSDO();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	vec3 indirectLight = vec3 (0);

	if (ssdoInterpolationEnabled == 1) {
		indirectLight = CalcInterpolatedIndirectDiffuseLight (in_position, in_normal, texCoord);
	}

	if (ssdoInterpolationEnabled == 0) {
		indirectLight = CalcScreenSpaceDirectionalOcclusion(in_position, in_normal) * ssdoIndirectIntensity;
	}

	out_color = indirectLight;
}
