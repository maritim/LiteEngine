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

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform sampler2D ssdoMap;
uniform sampler2D reflectionMap;
uniform sampler2D ssdoShadowMap;

#include "deferred.glsl"
#include "ScreenSpace/screenSpaceRayTracing.glsl"

vec3 CalcScreenSpaceReflection (vec3 in_position, vec3 in_normal, vec2 in_reflection)
{
	vec3 viewDirection = normalize (in_position);

	vec3 reflectionDirection = normalize (reflect (viewDirection, in_normal));

	vec3 reflection = texture2D (gDiffuseMap, in_reflection).xyz;

	float screenEdgeFade = 1.0f - clamp (length (vec2 (0.5f) - in_reflection), 0.0f, 0.5f) * 2.0f;

	vec3 reflectionViewPos = texture2D (gPositionMap, in_reflection).xyz;

	float d = distance (reflectionViewPos, in_position);

	return (reflection * screenEdgeFade *
		clamp (-reflectionDirection.z, 0.0f, 1.0f))
		/ max (d / 10, 1.0f);
}

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular,
	float in_shininess, vec3 in_ssdo, vec2 in_reflection, float in_shadow)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (vec3 (viewMatrix * vec4 (lightPosition, 0)));

	// Diffuse contribution
	float dCont = max (dot (in_normal, lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 directDiffuseColor = lightColor * dCont;

	vec3 surface2view = normalize (-in_position);
	vec3 reflection = reflect (-lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	vec3 directSpecularColor = lightColor * sCont;

	vec3 indirectDiffuseColor = in_ssdo;
	vec3 indirectSpecularColor = CalcScreenSpaceReflection (in_position, in_normal, in_reflection);

	directDiffuseColor = in_shadow * directDiffuseColor;
	directSpecularColor = in_shadow * directSpecularColor;

	return (directDiffuseColor + indirectDiffuseColor) * in_diffuse  +
		(directSpecularColor + indirectSpecularColor) * in_specular;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_specular = texture2D (gSpecularMap, texCoord).xyz;
	float in_shininess = texture2D (gSpecularMap, texCoord).w;
	vec3 in_ssdo = texture2D (ssdoMap, texCoord).xyz;
	vec2 in_reflection = texture2D (reflectionMap, texCoord).xy;
	float in_shadow = texture2D (ssdoShadowMap, texCoord).x;

	in_normal = normalize(in_normal);

	out_color = CalcDirectionalLight(in_position, in_normal, in_diffuse,
		in_specular, in_shininess, in_ssdo, in_reflection, in_shadow);
}