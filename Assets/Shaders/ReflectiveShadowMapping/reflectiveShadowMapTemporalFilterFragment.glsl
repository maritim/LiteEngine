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

uniform sampler2D lastIndirectMap;
uniform sampler2D indirectMap;

uniform mat4 reprojectionMatrix;

uniform vec2 rsmResolution;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;	
}

vec3 CalcClampNeighbourhood (vec3 lastIndirect, vec2 texCoord)
{
	vec3 color_min = vec3 (1.0);
	vec3 color_max = vec3 (0.0);

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 color = texture2D (indirectMap, texCoord + vec2 (i, j) / rsmResolution).xyz;

			color_min = vec3 (
				min (color.x, color_min.x),
				min (color.y, color_min.y),
				min (color.z, color_min.z));

			color_max = vec3 (
				max (color.x, color_max.x),
				max (color.y, color_max.y),
				max (color.z, color_max.z));
		}
	}

	return clamp (lastIndirect, color_min, color_max);
}

/*
 * Thanks to: http://twvideo01.ubm-us.net/o1/vault/gdc2016/Presentations/Pedersen_LasseJonFuglsang_TemporalReprojectionAntiAliasing.pdf
*/

vec3 CalcClipNeighbourhood (vec3 lastLight, vec2 texCoord)
{
	vec3 color_min = vec3 (1.0);
	vec3 color_max = vec3 (0.0);

	vec2 texelSize = vec2 (1.0) / screenSize;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 color = texture2D (indirectMap, texCoord + vec2 (i, j) * texelSize).xyz;

			color_min = min (color_min, color);
			color_max = max (color_max, color);
		}
	}

	vec3 p_clip = 0.5 * (color_max + color_min);
	vec3 e_clip = 0.5 * (color_max - color_min);

	vec3 v_clip = lastLight - p_clip;
	vec3 v_unit = v_clip / e_clip;
	vec3 a_unit = abs (v_unit);
	float max_unit = max (a_unit.x, max (a_unit.y, a_unit.z));

	if (max_unit > 1.0) {
		return p_clip + v_clip / max_unit;
	}

	return lastLight;
}

vec3 CalcIndirectDiffuseLight (vec3 in_position, vec3 in_normal, vec3 in_indirect, vec2 texCoord)
{
	vec4 lastProjected = reprojectionMatrix * vec4 (in_position, 1.0);
	vec2 lastTexCoord = lastProjected.xy / lastProjected.w;

	vec3 lastIndirect = texture2D (lastIndirectMap, lastTexCoord).xyz;

	vec3 clampedLastIndirect = CalcClampNeighbourhood (lastIndirect, texCoord);

	return mix (in_indirect, clampedLastIndirect, 0.99);
	// return (clampedLastIndirect * 7 + in_indirect) / 8.0;
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_indirect = texture2D (indirectMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcIndirectDiffuseLight(in_position, in_normal, in_indirect, texCoord);
}
