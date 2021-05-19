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
uniform vec2 cameraZLimits;

uniform vec2 ssdoResolution;
uniform float ssdoIndirectIntensity;
uniform float ssdoInterpolationScale;

#include "deferred.glsl"
#include "ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusion.glsl"

vec2 CalcTexCoordSSDO()
{
	return gl_FragCoord.xy / ssdoResolution;
}

void main()
{
	vec2 texCoord = CalcTexCoordSSDO();
	vec3 in_position = textureLod (gPositionMap, texCoord, log2 (1.0 / ssdoInterpolationScale)).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, log2 (1.0 / ssdoInterpolationScale)).xyz;

	in_normal = normalize(in_normal);

	vec3 indirectLight = CalcScreenSpaceDirectionalOcclusion(in_position, in_normal, texCoord);

	out_color = indirectLight * ssdoIndirectIntensity;
	// out_color = vec3 (1, 0, 0);
}
