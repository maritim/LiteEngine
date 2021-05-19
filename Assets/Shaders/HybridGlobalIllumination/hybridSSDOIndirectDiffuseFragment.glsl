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

uniform float hgiIntensity;
uniform int hgiInterpolationEnabled;
uniform float hgiInterpolationScale;
uniform float hgiMinInterpolationDistance;
uniform float hgiMinInterpolationAngle;
uniform int hgiDebugInterpolation;
uniform sampler2D ssdoIndirectDiffuseMap;

#include "HybridGlobalIllumination/hybridGlobalIllumination.glsl"

vec3 CalcInterpolatedIndirectDiffuseLight (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	if (in_position.z <= -cameraZLimits.y) {
		return vec3 (0);
	}

	vec3 indirectDiffuseLight = vec3 (0);

	vec3 interpolatedPosition = textureLod (gPositionMap, texCoord, log2 (1.0 / hgiInterpolationScale)).xyz;
	vec3 interpolatedNormal = textureLod (gNormalMap, texCoord, log2 (1.0 / hgiInterpolationScale)).xyz;

	if (distance (interpolatedPosition, in_position) < hgiMinInterpolationDistance
		&& dot (interpolatedNormal, in_normal) > hgiMinInterpolationAngle) {
		indirectDiffuseLight = texture (ssdoIndirectDiffuseMap, texCoord).xyz;
	} else {
		if (hgiDebugInterpolation == 1) {
			indirectDiffuseLight = vec3 (1, 0, 0);
		} else {
			indirectDiffuseLight = CalcHGIIndirectDiffuseLight(in_position, in_normal) * hgiIntensity;
		}
	}

	return indirectDiffuseLight;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	vec3 indirectLight = vec3 (0);

	if (hgiInterpolationEnabled == 1) {
		indirectLight = CalcInterpolatedIndirectDiffuseLight (in_position, in_normal, texCoord);
	}

	if (hgiInterpolationEnabled == 0) {
		indirectLight = CalcHGIIndirectDiffuseLight(in_position, in_normal) * hgiIntensity;
	}

	out_color = indirectLight;
}
