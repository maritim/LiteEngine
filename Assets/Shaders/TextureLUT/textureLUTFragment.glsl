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

uniform sampler2D postProcessMap;

uniform sampler3D lutTexture;
uniform float lutIntensity;

#include "deferred.glsl"

vec3 CalcTextureLUT (vec3 in_diffuse)
{
	vec3 color = texture (lutTexture, in_diffuse.rbg).xyz;

	return mix (color, in_diffuse, 1.0 - lutIntensity);
}

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;

	out_color = CalcTextureLUT (in_diffuse);
}
