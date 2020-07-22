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

uniform sampler2D temporalFilterMap;
uniform sampler2D postProcessMap;

#include "deferred.glsl"
#include "TemporalFiltering/temporalFiltering.glsl"

vec3 CalcTemporalFiltering (vec3 in_position, vec3 in_light, vec2 texCoord)
{
	vec2 lastTexCoord = CalcReprojectedTexCoord (in_position, texCoord);

	vec3 lastColor = texture2D (temporalFilterMap, lastTexCoord).xyz;

	vec3 clampedLastColor = CalcClipNeighbourhood (postProcessMap, screenSize, lastColor, texCoord);

	float weight = CalcBlendFactor (in_light, clampedLastColor);

	return mix (in_light, clampedLastColor, weight);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_light = texture2D (postProcessMap, CalcUnjitterTexCoord (texCoord)).xyz;

	out_color = CalcTemporalFiltering(in_position, in_light, texCoord);
}
