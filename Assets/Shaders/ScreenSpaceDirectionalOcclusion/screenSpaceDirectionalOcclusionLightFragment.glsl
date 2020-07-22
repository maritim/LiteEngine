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

uniform sampler2D ssdoMap;
uniform sampler2D ssdoShadowMap;

uniform int ssdoRayShadow;

#include "deferred.glsl"

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular,
	vec3 in_emissive, float in_shininess, vec3 in_ssdo, vec2 texCoord)
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

	vec3 indirectDiffuseColor = in_ssdo;

	float shadow = 1.0f;

	if (ssdoRayShadow > 0) {
		shadow = texture2D (ssdoShadowMap, texCoord).x;
	}

	directDiffuseColor = shadow * directDiffuseColor;
	directSpecularColor = shadow * directSpecularColor;

	return in_emissive + (directDiffuseColor + indirectDiffuseColor) * in_diffuse  +
		directSpecularColor * in_specular;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_specular = texture2D (gSpecularMap, texCoord).xyz;
	vec3 in_emissive = texture2D (gEmissiveMap, texCoord).xyz;
	float in_shininess = texture2D (gSpecularMap, texCoord).w;
	vec3 in_ssdo = texture2D (ssdoMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcDirectionalLight(in_position, in_normal, in_diffuse,
		in_specular, in_emissive, in_shininess, in_ssdo, texCoord);
}