#version 330

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRange;
uniform float lightSpotCutoff;
uniform float lightSpotOuterCutoff;

#include "deferred.glsl"

vec3 CalcSpotLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	// Vector direction from fragment to light source
	vec3 lightDir = lightPosition - in_position;

	// Distance from fragment to light source
	float dist2 = dot (lightDir, lightDir);

	// Normalize light direction
	lightDir = normalize (lightDir);

	float theta = dot (lightDir, normalize(-lightDirection));
	float epsilon = lightSpotCutoff - lightSpotOuterCutoff;
	float spotAttenuation = clamp ((theta - lightSpotOuterCutoff) / epsilon, 0.0, 1.0);

	// Compute point light attenuation over distance
	float attenuation = 1.0 / (dist2 + 1);
	attenuation = pow (clamp (1.0 - pow (dist2 / (lightRange * lightRange), 2), 0.0, 1.0), 2) * spotAttenuation;

	// Diffuse light intensity
	float diffuseLightIntensity = max (dot (in_normal, lightDir), 0.0);

	// Compute diffuse color
	vec3 diffuseColor = lightColor * in_diffuse * diffuseLightIntensity * attenuation;

	// Vector from fragment to camera position
	vec3 surface2view = normalize (-in_position);
	vec3 reflection = reflect (-lightDir, in_normal);

	// Specular light intensity
	float specularLightIntensity = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	// Compute specular color
	vec3 specularColor = lightColor * in_specular * specularLightIntensity * attenuation;

	return (diffuseColor + specularColor) * lightIntensity;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_specular = texture2D (gSpecularMap, texCoord).xyz;
	float in_shininess = texture2D (gSpecularMap, texCoord).w;

	in_normal = normalize(in_normal);

	out_color = CalcSpotLight(in_position, in_normal, in_diffuse, in_specular, in_shininess);
}