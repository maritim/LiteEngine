const int CASCADED_SHADOW_MAP_LEVELS = 4;

uniform int cascadesCount;

uniform sampler2DShadow shadowMaps[CASCADED_SHADOW_MAP_LEVELS];

uniform mat4 lightSpaceMatrices[CASCADED_SHADOW_MAP_LEVELS];

uniform float clipZLevels[CASCADED_SHADOW_MAP_LEVELS];

uniform float shadowBias;

/*
 * Shadow Calculation
 * Thanks to: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
*/

float ShadowCalculation (vec4 lightSpacePos, int cascadedLevel)
{
	// Check whether current frag pos is in shadow
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(shadowMaps [cascadedLevel], 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			vec3 samplePos = vec3 (lightSpacePos.xy + vec2 (x, y) * texelSize, lightSpacePos.z - shadowBias);
			shadow += texture (shadowMaps [cascadedLevel], samplePos, shadowBias);
		}    
	}

	shadow /= 9.0;

	return shadow;
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
