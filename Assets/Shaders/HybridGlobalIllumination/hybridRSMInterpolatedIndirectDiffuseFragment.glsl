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

uniform vec2 hgiResolution;
uniform float hgiIntensity;
uniform float hgiInterpolationScale;

#include "HybridGlobalIllumination/hybridGlobalIllumination.glsl"

vec2 CalcTexCoordHGI ()
{
	return gl_FragCoord.xy / hgiResolution;	
}

void main()
{
	vec2 texCoord = CalcTexCoordHGI();
	vec3 in_position = textureLod (gPositionMap, texCoord, log2 (1.0 / hgiInterpolationScale)).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, log2 (1.0 / hgiInterpolationScale)).xyz;

	in_normal = normalize(in_normal);

	vec3 indirectDiffuseLight = CalcViewHGIIndirectDiffuseLight(in_position, in_normal);

	out_color = clamp (indirectDiffuseLight * hgiIntensity, 0.0, 1.0);
}
