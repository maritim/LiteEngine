const int CASCADED_SHADOW_MAP_LEVELS = 4;

uniform int cascadesCount;

uniform sampler2DShadow shadowMap;

uniform mat4 lightSpaceMatrices[CASCADED_SHADOW_MAP_LEVELS];

uniform float clipZLevels[CASCADED_SHADOW_MAP_LEVELS];

uniform float shadowBias;

/*
 * Shadow Calculation
 * Thanks to: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
*/

float ShadowCalculation (vec4 lightSpacePos)
{
	// Check whether current frag pos is in shadow
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			vec3 samplePos = vec3 (lightSpacePos.xy + vec2 (x, y) * texelSize, lightSpacePos.z - shadowBias);
			shadow += texture (shadowMap, samplePos, shadowBias);
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
	int shadowCascadeLevel = GetShadowCascadeLevel (depth);

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrices [shadowCascadeLevel] * inverseViewMatrix * vec4 (viewPosition, 1.0f);
	float shadow = ShadowCalculation (lightSpacePos);

	return shadow;
}

float CalcDirectionalShadowContributionDirect (vec3 viewPosition)
{
	// Calculate shadow level
	vec4 clipPos = (projectionMatrix * vec4 (viewPosition, 1.0));
	float depth = clipPos.z / clipPos.w;
	int shadowCascadeLevel = GetShadowCascadeLevel (depth);

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrices [shadowCascadeLevel] * inverseViewMatrix * vec4 (viewPosition, 1.0f);

	vec3 samplePos = vec3 (lightSpacePos.xy, lightSpacePos.z - shadowBias);
	float shadow = texture (shadowMap, samplePos, shadowBias);

	return shadow;
}

