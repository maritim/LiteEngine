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

uniform vec3 sceneAmbient;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightSpecularColor;

uniform vec3 attenuationComp;

uniform vec2 screenSize;

uniform sampler2D shadowMap;

uniform mat4 lightSpaceMatrix;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

/*
 * Shadow Calculation
 * Thanks to: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
*/

float ShadowCalculation (vec4 lightSpacePos)
{
    // perform perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture (shadowMap, projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcPointLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	// Vector from Light Source to Fragment
	vec3 lightDirection = in_position - lightPosition;

	// Distance from Light Source to Fragment
	float dist = length (lightDirection);
	
	// Normalize light vector from light source
	lightDirection = normalize (lightDirection);

	// Calculate Point Light Attenuation over distance
	float attenuation = 1.0 / (attenuationComp.x + attenuationComp.y * dist + attenuationComp.z * dist * dist);

	// Diffuse contribution
	float dCont = max (dot (in_normal, lightDirection), 0.0);

	// Calculate Diffuse Color
	vec3 diffuseColor = lightColor * in_diffuse * dCont * attenuation;

	// Vector from Camera Positon to Fragment
	vec3 surface2view = normalize (cameraPosition - in_position);
	vec3 reflection = reflect (-lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), 3);

	vec3 specularColor = lightSpecularColor * in_diffuse * sCont;

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrix * in_position;
	float shadow = ShadowCalculation (lightSpacePos);

	return (1.0 - shadow) * (diffuseColor + specularColor);
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