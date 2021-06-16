uniform sampler2DShadow shadowMap;

uniform mat4 lightSpaceMatrix;

uniform float shadowBias;

float ShadowCalculationPCF (vec4 lightSpacePos)
{
	// perform perspective divide
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	// Check whether current frag pos is in shadow
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			vec3 samplePos = vec3 (projCoords.xy + vec2 (x, y) * texelSize, projCoords.z - shadowBias);
			shadow += texture (shadowMap, samplePos, shadowBias);
		}    
	}

	shadow /= 9.0;

	return shadow;
}

float CalcShadowContribution (vec3 viewPosition)
{
	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrix * inverseViewMatrix * vec4 (viewPosition, 1.0f);
	float shadow = ShadowCalculationPCF (lightSpacePos);

	return shadow;
}

float CalcShadowContributionDirect (vec3 viewPosition)
{
	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrix * inverseViewMatrix * vec4 (viewPosition, 1.0f);

	// perform perspective divide
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	vec3 samplePos = vec3 (projCoords.xy * texelSize, projCoords.z - shadowBias);

	float shadow = texture (shadowMap, samplePos, shadowBias);

	return shadow;
}
