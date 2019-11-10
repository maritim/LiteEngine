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

uniform sampler2DShadow rsmShadowMap;
uniform sampler2D rsmPositionMap;
uniform sampler2D rsmNormalMap;
uniform sampler2D rsmFluxMap;

uniform mat4 rsmLightSpaceMatrix;

uniform float rsmBias;

uniform sampler2D indirectMap;

#include "deferred.glsl"
#include "AmbientLight/ambientLight.glsl"

vec3 CalcDirectDiffuseLight (vec3 in_position, vec3 in_normal)
{
	// Diffuse contribution
	float dCont = max (dot (in_normal, -lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuseColor = lightColor * dCont;

	return diffuseColor;
}

vec3 CalcDirectSpecularLight (vec3 in_position, vec3 in_normal, float in_shininess)
{
	vec3 surface2view = normalize (-in_position);
	vec3 reflection = reflect (lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	vec3 specularColor = lightColor * sCont;

	return specularColor;
}

float CalcDirectionalShadowContribution (vec3 worldSpacePos)
{
	vec4 lightSpacePos = rsmLightSpaceMatrix * inverseViewMatrix * vec4 (worldSpacePos, 1.0);
	vec3 rsmProjCoords = lightSpacePos.xyz / lightSpacePos.w;

	if(rsmProjCoords.z > 1.0)
		return 0.0;

	rsmProjCoords = rsmProjCoords * 0.5 + 0.5;

	// Check whether current frag pos is in shadow
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(rsmShadowMap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			vec3 samplePos = vec3 (rsmProjCoords.xy + vec2 (x, y) * texelSize, rsmProjCoords.z - rsmBias);
			shadow += texture (rsmShadowMap, samplePos, 0);
		}
	}

	shadow /= 9.0;

	return shadow;
}

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess, vec3 in_indirect)
{
	vec3 directDiffuseColor = CalcDirectDiffuseLight (in_position, in_normal);

	float shadow = CalcDirectionalShadowContribution (in_position);

	directDiffuseColor = shadow * directDiffuseColor;

	vec3 directSpecularColor = CalcDirectSpecularLight (in_position, in_normal, in_shininess);

	directSpecularColor = shadow * directSpecularColor;

	// Calculate ambient light
	vec3 ambientColor = in_diffuse * CalcAmbientLight ();

	// return in_indirect;
	return (directDiffuseColor * in_diffuse
			+ directSpecularColor * in_specular) * lightIntensity + (in_indirect * in_diffuse) + ambientColor;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_specular = texture2D (gSpecularMap, texCoord).xyz;
	float in_shininess = texture2D (gSpecularMap, texCoord).w;
	vec3 in_indirect = texture2D (indirectMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcDirectionalLight(in_position, in_normal, in_diffuse, in_specular, in_shininess, in_indirect);
} 