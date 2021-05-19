#version 330

layout(location = 0) out float out_color;

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

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform int shadowConeEnabled;
uniform float shadowConeRatio;
uniform float shadowConeDistance;

#include "deferred.glsl"
#include "ShadowMap/shadowMap.glsl"
#include "VoxelConeTracing/voxelConeTracing.glsl"

float CalcVoxelConeTracingShadow (vec3 in_position, vec3 in_normal)
{
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 voxelPos = GetPositionInVolume (worldPosition) + worldNormal * originBias;

	vec3 lightDir = vec3 (inverseViewMatrix * vec4 (-lightDirection, 0.0));

	float occlusion = voxelTraceConeOcclusion (voxelPos, lightDir, shadowConeRatio, shadowConeDistance);

	return 1.0 - occlusion;
}

float CalcShadow (vec3 in_position, vec3 in_normal)
{
	return shadowConeEnabled > 0 ?
		CalcVoxelConeTracingShadow (in_position, in_normal) :
		CalcShadowContribution (in_position);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcShadow (in_position, in_normal);
}
