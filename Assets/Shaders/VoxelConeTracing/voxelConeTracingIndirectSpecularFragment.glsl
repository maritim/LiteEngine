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

uniform float vctIndirectIntensity;
uniform float specularConeRatio;
uniform float specularConeDistance;
uniform float specularOriginBias;

#include "deferred.glsl"
#include "VoxelConeTracing/voxelConeTracing.glsl"

// Calculates indirect specular light using voxel cone tracing.
vec3 CalcIndirectSpecularLight (vec3 in_position, vec3 in_normal, float in_shininess)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 specularLight = vec3 (0.0);

	vec3 eyeToFragment = normalize (worldPosition - cameraPosition);
	vec3 reflectionDir = reflect (eyeToFragment, worldNormal);

	// float specularConeRatio = 1.0 / in_shininess;

	vec3 reflectTraceOrigin = GetPositionInVolume (worldPosition) + worldNormal * specularOriginBias;
	specularLight = voxelTraceCone (reflectTraceOrigin, reflectionDir, specularConeRatio, specularConeDistance).xyz;

	return specularLight * vctIndirectIntensity;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;
	float in_shininess = textureLod (gSpecularMap, texCoord, 0).w;

	in_normal = normalize(in_normal);

	out_color = CalcIndirectSpecularLight (in_position, in_normal, in_shininess);
} 