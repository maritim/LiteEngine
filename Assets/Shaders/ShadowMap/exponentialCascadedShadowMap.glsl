const int CASCADED_SHADOW_MAP_LEVELS = 4;

uniform int cascadesCount;

uniform sampler2D shadowMaps[CASCADED_SHADOW_MAP_LEVELS];

uniform mat4 lightSpaceMatrices[CASCADED_SHADOW_MAP_LEVELS];

uniform float clipZLevels[CASCADED_SHADOW_MAP_LEVELS];

uniform float exponential;

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
	float currentDepth = exp (-exponential * projCoords.z);

	float shadow = clamp (closestDepth * currentDepth, 0.0, 1.0);

	return 1.0 - shadow;
}

int GetShadowCascadeLevel (float depth)
{
	for (int index = 0 ; index < cascadesCount ; index++) {
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
