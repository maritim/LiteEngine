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
uniform float rsmInterpolationScale;
uniform float rsmMinInterpolationDistance;
uniform float rsmMinInterpolationAngle;
uniform int rsmDebugInterpolation;

uniform sampler2D indirectDiffuseMap;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;	
}

vec3 CalcInterpolatedIndirectDiffuseLight (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	if (dot (in_position, in_position) == 0) {
		return vec3 (0);
	}

	vec3 indirectDiffuseLight = vec3 (0);

	vec3 interpolatedPosition = textureLod (gPositionMap, texCoord, log2 (1.0 / rsmInterpolationScale)).xyz;
	vec3 interpolatedNormal = textureLod (gNormalMap, texCoord, log2 (1.0 / rsmInterpolationScale)).xyz;

	if (distance (interpolatedPosition, in_position) < rsmMinInterpolationDistance
		&& dot (interpolatedNormal, in_normal) > rsmMinInterpolationAngle) {
		indirectDiffuseLight = texture (indirectDiffuseMap, texCoord).xyz;
	} else {
		if (rsmDebugInterpolation == 1) {
			indirectDiffuseLight = vec3 (1, 0, 0);
		} else {
			indirectDiffuseLight = clamp (CalcViewIndirectDiffuseLight(in_position, in_normal) * rsmIntensity, 0.0, 1.0);
		}
	}

	return indirectDiffuseLight;
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcInterpolatedIndirectDiffuseLight (in_position, in_normal, texCoord);
}
