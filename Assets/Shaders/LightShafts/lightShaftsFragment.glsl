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

uniform sampler2D volumetricLightMap;
uniform sampler2D postProcessMap;

#include "deferred.glsl"

vec3 CalcLightShafts (const in vec2 texCoord)
{
	vec2 deltaTextCoord = vec2( texCoord - lightPositionOnScreen.xy );
	deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
	float illuminationDecay = 1.0;

	for (int i = 0; i < NUM_SAMPLES; i++) {
		texCoord -= deltaTextCoord;
		vec4 sample = texture (gPositionMap, texCoord);
		sample *= illuminationDecay * weight;
		gl_FragColor += sample;
		illuminationDecay *= decay;
	}
}

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture2D (postProcessMap, texCoord).xyz;
	vec3 in_volLighting = texture2D (volumetricLightMap, texCoord).xyz;

	out_color = in_diffuse + in_volLighting;
}
