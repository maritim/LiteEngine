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

uniform sampler2D taaMap;
uniform sampler2D postProcessMap;

uniform vec2 frustumJitter;
uniform mat4 reprojectionMatrix;

#include "deferred.glsl"

vec3 CalcClampNeighbourhood (vec3 lastLight, vec2 texCoord)
{
	vec3 color_min = vec3 (1.0);
	vec3 color_max = vec3 (-1.0);

	vec2 texelSize = vec2 (1.0) / screenSize;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 color = texture2D (postProcessMap, texCoord + vec2 (i, j) * texelSize).xyz;

			color_min = min (color_min, color);
			color_max = max (color_max, color);
		}
	}

	return clamp (lastLight, color_min, color_max);
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
			vec3 color = texture2D (postProcessMap, texCoord + vec2 (i, j) * texelSize).xyz;

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

vec2 Unjitter (vec2 texCoord)
{
	return texCoord - frustumJitter;
}

vec3 CalcTemporalAntialiasing (vec3 in_position, vec3 in_light, vec2 texCoord)
{
	vec4 lastProjected = reprojectionMatrix * vec4 (in_position, 1.0);
	vec2 lastTexCoord = lastProjected.xy / lastProjected.w;

	vec3 lastColor = texture2D (taaMap, lastTexCoord).xyz;

	vec3 clampedLastColor = CalcClipNeighbourhood (lastColor, texCoord);

	return mix (in_light, clampedLastColor, 0.95);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec4 in_position = texture2D (gPositionMap, texCoord);
	vec3 in_light = texture2D (postProcessMap, Unjitter (texCoord)).xyz;

	out_color = CalcTemporalAntialiasing(in_position.xyz, in_light, texCoord);
}
