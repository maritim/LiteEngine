#version 330 core

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform float ssrIntensity;

uniform sampler2D postProcessMap;

uniform sampler2D reflectionMap;

#include "deferred.glsl"

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture (gNormalMap, texCoord).xyz;
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;
	vec3 in_specular = texture (gSpecularMap, texCoord).xyz;
	vec2 in_reflection = texture (reflectionMap, texCoord).xy;

	in_normal = normalize (in_normal);

	vec3 reflection = vec3 (0.0f);

	if (length (in_reflection) > 0.0f) {
		reflection = texture (postProcessMap, in_reflection).xyz;
	}

	out_color = reflection * ssrIntensity;
}
