#version 330

layout(location = 0) out float out_color;

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

uniform sampler2D temporalFilterMap;
uniform sampler2D ambientOcclusionMap;

uniform vec2 ssaoResolution;

#include "deferred.glsl"
#include "TemporalFiltering/temporalFiltering.glsl"

vec2 CalcTexCoordSSAO()
{
	return gl_FragCoord.xy / ssaoResolution;
}

void main()
{
	vec2 texCoord = CalcTexCoordSSAO();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;

	out_color = CalcTemporalFiltering(temporalFilterMap, ambientOcclusionMap,
		ssaoResolution, in_position, texCoord, false).x;
}
