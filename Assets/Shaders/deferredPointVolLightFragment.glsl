#version 330

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 attenuationComp;

uniform vec2 screenSize;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

vec3 CalcPointLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	// Vector direction from fragment to light source
	vec3 lightDirection = vec3 (viewMatrix * vec4 (lightPosition, 1)) - in_position;

	// Distance from fragment to light source
	float dist = length (lightDirection);

	// Normalize light direction
	lightDirection = normalize (lightDirection);

	// Compute point light attenuation over distance
	float attenuation = 1.0 / (attenuationComp.x + attenuationComp.y * dist + attenuationComp.z * dist * dist);

	// Diffuse light intensity
	float diffuseLightIntensity = max (dot (in_normal, lightDirection), 0.0);

	// Compute diffuse color
	vec3 diffuseColor = lightColor * in_diffuse * diffuseLightIntensity * attenuation;

	// Vector from fragment to camera position
	vec3 surface2view = normalize (-in_position);
	vec3 reflection = reflect (-lightDirection, in_normal);

	// Specular light intensity
	float specularLightIntensity = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	// Compute specular color
	vec3 specularColor = lightColor * in_diffuse * in_specular * specularLightIntensity * attenuation;

	return diffuseColor;// + specularColor;
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

	out_color = CalcPointLight(in_position, in_normal, in_diffuse, in_specular, in_shininess);
} 