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

uniform sampler2D lastIndirectDiffuseMap;
uniform sampler2D indirectDiffuseMap;

uniform vec2 rsmResolution;

#include "deferred.glsl"
#include "TemporalFiltering/temporalFiltering.glsl"

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;
}

vec3 CalcIndirectDiffuseLight (vec3 in_position, vec3 in_indirect, vec2 texCoord)
{
	vec2 lastTexCoord = CalcReprojectedTexCoord (in_position, texCoord);

	vec3 lastIndirect = texture2D (lastIndirectDiffuseMap, lastTexCoord).xyz;

	vec3 clampedLastIndirect = CalcClipNeighbourhood (indirectDiffuseMap, rsmResolution, lastIndirect, texCoord);

	return mix (in_indirect, clampedLastIndirect, 0.99);
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_indirect = texture2D (indirectDiffuseMap, texCoord).xyz;

	out_color = CalcIndirectDiffuseLight(in_position, in_indirect, texCoord);
}
