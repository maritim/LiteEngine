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

uniform sampler2D lightShaftsMap;
uniform sampler2D postProcessMap;

#include "deferred.glsl"

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture2D (postProcessMap, texCoord).xyz;
	vec3 in_lightShafts = texture2D (lightShaftsMap, texCoord).xyz;

	out_color = in_diffuse + in_lightShafts;
}
