#version 330

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;
uniform mat3 inverseNormalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightSpecularColor;

uniform vec3 attenuationComp;

uniform vec2 screenSize;

const int CASCADED_SHADOW_MAP_LEVELS = 4;

uniform sampler2D shadowMaps[CASCADED_SHADOW_MAP_LEVELS];

uniform mat4 lightSpaceMatrices[CASCADED_SHADOW_MAP_LEVELS];

uniform float clipZLevels[CASCADED_SHADOW_MAP_LEVELS];

uniform sampler2D rsmShadowMap;
uniform sampler2D rsmPositionMap;
uniform sampler2D rsmNormalMap;
uniform sampler2D rsmFluxMap;

uniform mat4 rsmLightSpaceMatrix;

uniform int rsmSamplesCount;

uniform vec2 rsmSample[200];

const float rsmIntensity = 1.0f;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

/*
 * Shadow Calculation
 * Thanks to: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
*/

float ShadowCalculation (vec4 lightSpacePos, int cascadedLevel)
{
    // perform perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	if(projCoords.z > 1.0)
        return 0.0;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture (shadowMaps [cascadedLevel], projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
	float bias = 0.0002;

    // Check whether current frag pos is in shadow
    float shadow = 0.5;

	vec2 texelSize = 1.0 / textureSize(shadowMaps [cascadedLevel], 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMaps [cascadedLevel], projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	shadow /= 9.0;

    return shadow;
}

int GetShadowCascadeLevel (float depth)
{
	for (int index = 0 ; index < CASCADED_SHADOW_MAP_LEVELS ; index++) {
        if (depth <= clipZLevels [index]) {
			return index;
        }
    }
}

float CalcShadowContribution (vec3 in_position)
{
	// Calculate shadow level
	vec4 clipPos = (viewProjectionMatrix * vec4 (in_position, 1.0));
	float depth = clipPos.z / clipPos.w;
	int shadowCascadedLevel = GetShadowCascadeLevel (depth);

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrices [shadowCascadedLevel] * vec4 (in_position, 1.0f);
	float shadow = ShadowCalculation (lightSpacePos, shadowCascadedLevel);

	return shadow;
}

vec3 CalcDirectDiffuseLight (vec3 in_position, vec3 in_normal)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	// Diffuse contribution
	float dCont = max (dot (in_normal, lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuseColor = lightColor * dCont;

	return diffuseColor;
}

//vec3 CalcDirectSpecularLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
//{
//	vec3 surface2view = normalize (cameraPosition - in_position);
//	vec3 reflection = reflect (-lightDirection, in_normal);

//	// Specular contribution
//	float sCont = pow (max (dot (surface2view, reflection), 0.0), 1.0);

//	vec3 specularColor = lightSpecularColor * sCont;

//	return specularColor;
//}

/*
 * Indirect Illumination Calculation
 * Thanks to: http://ericpolman.com/reflective-shadow-maps-part-2-the-implementation/
*/

vec3 CalcIndirectDiffuseLight (vec3 worldSpacePos, vec3 worldSpaceNormal)
{
	vec3 indirectColor = vec3 (0.0);
	float rMax = 0.07f;

	vec4 lightSpacePos = rsmLightSpaceMatrix * vec4 (worldSpacePos, 1.0);
	vec3 rsmProjCoords = lightSpacePos.xyz / lightSpacePos.w;

	rsmProjCoords = rsmProjCoords * 0.5 + 0.5;

	for (int index = 0; index < rsmSamplesCount; index ++) {
		vec2 rnd = rsmSample [index];

		vec2 coords = rsmProjCoords.xy + rnd * rMax;

		vec3 rsmWorldSpacePos = texture2D (rsmPositionMap, coords).xyz;
		vec3 rsmWorldSpaceNormal = texture2D (rsmNormalMap, coords).xyz;
		vec3 rsmFlux = texture2D (rsmFluxMap, coords).xyz;

		vec3 result = rsmFlux *
			((max (0.0, dot (rsmWorldSpaceNormal, worldSpacePos - rsmWorldSpacePos))
				* max (0.0, dot (worldSpaceNormal, rsmWorldSpacePos - worldSpacePos)))
			/ pow (length (worldSpacePos - rsmWorldSpacePos), 4.0));

		float dist = length (rnd);

		result = result * dist * dist;

		indirectColor += result;
	}

	return indirectColor * rsmIntensity;
}

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 directDiffuseColor = CalcDirectDiffuseLight (worldPosition, worldNormal);

	float shadow = CalcShadowContribution (worldPosition);

	directDiffuseColor = (1.0 - shadow) * (directDiffuseColor);

	vec3 indirectDiffuseColor = CalcIndirectDiffuseLight (worldPosition, worldNormal);

	return (directDiffuseColor + indirectDiffuseColor) * in_diffuse;
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

	out_color = CalcDirectionalLight(in_position, in_normal, in_diffuse, in_specular, in_shininess);
} 