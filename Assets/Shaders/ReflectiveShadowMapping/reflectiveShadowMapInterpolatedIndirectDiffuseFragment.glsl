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

uniform vec2 rsmResolution;
uniform float rsmIntensity;
uniform float rsmInterpolationScale;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;	
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = textureLod (gPositionMap, texCoord, log2 (1.0 / rsmInterpolationScale)).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, log2 (1.0 / rsmInterpolationScale)).xyz;

	in_normal = normalize(in_normal);

	vec3 indirectDiffuseLight = CalcViewIndirectDiffuseLight(in_position, in_normal);

	out_color = clamp (indirectDiffuseLight * rsmIntensity, 0.0, 1.0);
}
