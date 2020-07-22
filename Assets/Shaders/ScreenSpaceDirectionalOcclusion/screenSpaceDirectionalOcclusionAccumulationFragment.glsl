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

uniform float bloomIntensity;

uniform sampler2D postProcessMap;
uniform sampler2D ssdoMap;

#include "deferred.glsl"
#include "AmbientLight/ambientLight.glsl"

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_postProcess = texture2D (postProcessMap, texCoord).xyz;
	vec3 in_ssdo = texture2D (ssdoMap, texCoord).xyz;

	float ambientOcclusion = 1.0;

	if (ambientOcclusionEnabled == 1) {
		float in_ao = texture2D (ambientOcclusionMap, texCoord).x;

		ambientOcclusion = in_ao;
	}

	out_color = in_postProcess + in_diffuse * in_ssdo * ambientOcclusion;// * bloomIntensity;
}
