#version 330 core

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

#include "deferred.glsl"

#define SUN_SIZE 0.1

vec3 CalcDirectionalLightSource (const in vec2 texCoord)
{
	vec3 lightPosition = -lightDirection;
	vec4 lightPositionTexcoord = projectionMatrix * vec4 (lightPosition, 1.0);
	lightPositionTexcoord.xyz = lightPositionTexcoord.xyz * 0.5 + 0.5;
	lightPositionTexcoord.xyz /= lightPositionTexcoord.w;

	float aspect = screenSize.x / screenSize.y;

	lightPositionTexcoord.x *= aspect;
	vec2 currentTexcoord = vec2 (texCoord.x * aspect, texCoord.y);

	float dist = distance (lightPositionTexcoord.xy, currentTexcoord);

	if (dist > SUN_SIZE) {
		return vec3 (0.0);
	}

	return lightColor * lightIntensity;
}

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;

	out_color = vec3 (0.0);

	if (in_position.z <= -cameraZLimits.y) {
		out_color = CalcDirectionalLightSource (texCoord);
	}
}
