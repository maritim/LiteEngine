const int CASCADED_SHADOW_MAP_LEVELS = 4;

uniform sampler2D shadowMaps[CASCADED_SHADOW_MAP_LEVELS];

uniform mat4 lightSpaceMatrices[CASCADED_SHADOW_MAP_LEVELS];

uniform float clipZLevels[CASCADED_SHADOW_MAP_LEVELS];

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
	float shadow = 0.0;

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

float CalcDirectionalShadowContribution (vec3 viewPosition)
{
	// Calculate shadow level
	vec4 clipPos = (projectionMatrix * vec4 (viewPosition, 1.0));
	float depth = clipPos.z / clipPos.w;
	int shadowCascadedLevel = GetShadowCascadeLevel (depth);

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrices [shadowCascadedLevel] * inverseViewMatrix * vec4 (viewPosition, 1.0f);
	float shadow = ShadowCalculation (lightSpacePos, shadowCascadedLevel);

	return shadow;
}
