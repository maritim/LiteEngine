#version 330

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform sampler2D ssdoShadowMap;

uniform int ssdoRayShadow;

#include "deferred.glsl"
#include "ShadowMap/cascadedShadowMap.glsl"
#include "AmbientLight/ambientLight.glsl"

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular,
	vec3 in_emissive, float in_shininess, vec2 texCoord)
{
	// Diffuse contribution
	float dCont = max (dot (in_normal, -lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 directDiffuseColor = lightColor * dCont;

	vec3 surface2view = normalize (-in_position);
	vec3 reflection = reflect (lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	vec3 directSpecularColor = lightColor * sCont;

	float shadow = 1.0f;

	if (ssdoRayShadow == 0) {
		shadow = CalcDirectionalShadowContribution (in_position);
	} else {
		shadow = texture2D (ssdoShadowMap, texCoord).x;
	}

	directDiffuseColor = shadow * directDiffuseColor;
	directSpecularColor = shadow * directSpecularColor;

	// Calculate ambient light
	vec3 ambientColor = in_diffuse * CalcAmbientLight ();

	return in_emissive + directDiffuseColor * in_diffuse  +
		directSpecularColor * in_specular + ambientColor;
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

	in_normal = normalize(in_normal);

	out_color = CalcDirectionalLight(in_position, in_normal, in_diffuse,
		in_specular, in_emissive, in_shininess, texCoord);
}