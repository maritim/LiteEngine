uniform vec2 frustumJitter;
uniform mat4 reprojectionMatrix;

#include "colorSpace.glsl"

vec2 CalcUnjitterTexCoord (vec2 texCoord)
{
	return texCoord - frustumJitter;
}

vec2 CalcReprojectedTexCoord (const in vec3 in_position, const in vec2 texCoord)
{
	if (length (in_position) == 0.0) {
		return texCoord;
	}

	vec4 lastProjected = reprojectionMatrix * vec4 (in_position, 1.0);
	vec2 lastTexCoord = lastProjected.xy / lastProjected.w;

	bvec2 a = greaterThan(lastTexCoord, vec2(1.0, 1.0));
	bvec2 b = lessThan(lastTexCoord, vec2(0.0, 0.0));

	if (any(bvec2(any(a), any(b)))) {
		return texCoord;
	}

	return lastTexCoord;
}

vec3 CalcClampNeighbourhood (const in sampler2D currentMap, const in vec2 screenSize, const in vec3 lastLight, const in vec2 texCoord)
{
	vec3 color_min = vec3 (1.0);
	vec3 color_max = vec3 (-1.0);

	vec2 texelSize = vec2 (1.0) / screenSize;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 color = texture2D (currentMap, texCoord + vec2 (i, j) * texelSize).xyz;

			color_min = min (color_min, color);
			color_max = max (color_max, color);
		}
	}

	return clamp (lastLight, color_min, color_max);
}

/*
 * Thanks to: http://twvideo01.ubm-us.net/o1/vault/gdc2016/Presentations/Pedersen_LasseJonFuglsang_TemporalReprojectionAntiAliasing.pdf
*/

vec3 CalcClipNeighbourhood (const in sampler2D currentMap, const in vec2 resolution, const in vec3 lastLight, const in vec2 texCoord)
{
	vec3 color_min = vec3 (1.0);
	vec3 color_max = vec3 (0.0);

	vec2 texelSize = vec2 (1.0) / resolution;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 color = texture2D (currentMap, texCoord + vec2 (i, j) * texelSize).xyz;
			color = RGBToYCoCg (color);

			color_min = min (color_min, color);
			color_max = max (color_max, color);
		}
	}

	vec3 colorSample = RGBToYCoCg (lastLight);

	vec3 p_clip = 0.5 * (color_max + color_min);
	vec3 e_clip = 0.5 * (color_max - color_min);

	vec3 v_clip = colorSample - p_clip;
	vec3 v_unit = v_clip / e_clip;
	vec3 a_unit = abs (v_unit);
	float max_unit = max (a_unit.x, max (a_unit.y, a_unit.z));

	if (max_unit > 1.0) {
		return YCoCgToRGB (p_clip + v_clip / max_unit);
	}

	return lastLight;
}

/*
 * Thanks to: https://de45xmedrsdbp.cloudfront.net/Resources/files/TemporalAA_small-59732822.pdf
*/

float CalcBlendFactor (const in vec3 currentColor, const in vec3 historyColor)
{
	return 0.95;

	float lum0 = CalcLuminance (currentColor);
	float lum1 = CalcLuminance (historyColor);

	float unbiased_diff = abs(lum0 - lum1) / max(lum0, max(lum1, 0.2));
	float unbiased_weight = 1.0 - unbiased_diff;
	float unbiased_weight_sqr = unbiased_weight * unbiased_weight;
	float weight = mix (0.8, 0.95, unbiased_weight_sqr);

	return weight;
}
