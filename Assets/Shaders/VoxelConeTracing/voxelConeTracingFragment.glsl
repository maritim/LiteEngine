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

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform float shadowConeRatio;
uniform float shadowConeDistance;

#include "deferred.glsl"
#include "IndirectLight/indirectLight.glsl"
#include "SubsurfaceScattering/subsurfaceScattering.glsl"
#include "VoxelConeTracing/voxelConeTracing.glsl"

/*
 * Calculate  a vector that is orthogonal to u.
*/

vec3 Orthogonal(vec3 u)
{
	u = normalize(u);

	vec3 v = vec3(0.0, 1.0, 0.0);

	return abs(dot(u, v)) > 0.99 ? cross(u, vec3(0, 0, 1)) : cross(u, v);
}

vec3 CalcDirectDiffuseLight (vec3 in_position, vec3 in_normal)
{
	// Diffuse contribution
	float dCont = max (dot (in_normal, -lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuseColor = lightColor * dCont * lightIntensity;

	return diffuseColor;
}

vec3 CalcDirectSpecularLight (vec3 in_position, vec3 in_normal, float in_shininess)
{
	vec3 surface2view = normalize (-in_position);
	vec3 reflection = reflect (lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	vec3 specularColor = lightColor * sCont * lightIntensity;

	return specularColor;
}

float CalcShadow (vec3 in_position, vec3 in_normal)
{
	vec3 voxelPos = GetPositionInVolume (in_position) + in_normal * originBias;

	vec3 lightDir = vec3 (inverseViewMatrix * vec4 (-lightDirection, 0.0));

	float occlusion = voxelTraceConeOcclusion (voxelPos, lightDir, shadowConeRatio, shadowConeDistance);

	return 1.0 - occlusion;
}

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse,
	vec3 in_specular, vec3 in_emissive, float in_shininess,
	float in_transparency)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 directDiffuseColor = CalcDirectDiffuseLight (in_position, in_normal);
	vec3 directSpecularColor = CalcDirectSpecularLight (in_position, in_normal, in_shininess);

	float shadow = CalcShadow (worldPosition, worldNormal);

	directDiffuseColor = shadow * directDiffuseColor;
	directSpecularColor = shadow * directSpecularColor;

	vec3 indirectDiffuseColor = CalcIndirectDiffuseLight ();
	vec3 indirectSpecularColor = CalcIndirectSpecularLight ();

	vec3 indirectSubsurfaceScatteringLight = CalcSubsurfaceScatteringLight ();

	// Calculate ambient light
	vec3 ambientColor = CalcAmbientLight ();

	// return indirectSpecularColor;
	// return vec3 (shadow);
	// return indirectDiffuseColor;
	// return indirectSpecularColor;
	// return refractiveColor;
	vec3 directLight = in_emissive + directDiffuseColor * in_diffuse + directSpecularColor * in_specular;
	vec3 indirectLight = indirectDiffuseColor * in_diffuse
		   + indirectSpecularColor * in_specular
		   + ambientColor * in_diffuse;

	return mix (indirectLight + directLight, indirectSubsurfaceScatteringLight, in_transparency);
	// return mix (indirectLight, indirectRefractiveColor, in_transparency) + directLight;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_diffuse = textureLod (gDiffuseMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;
	vec3 in_specular = textureLod (gSpecularMap, texCoord, 0).xyz;
	vec3 in_emissive = textureLod (gEmissiveMap, texCoord, 0).xyz;
	float in_shininess = textureLod (gSpecularMap, texCoord, 0).w;
	float in_transparency = textureLod (gDiffuseMap, texCoord, 0).w;

	in_normal = normalize(in_normal);

	out_color = CalcDirectionalLight (in_position, in_normal, in_diffuse,
		in_specular, in_emissive, in_shininess, in_transparency);
} 