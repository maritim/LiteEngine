uniform sampler2DShadow shadowMap;

uniform mat4 lightSpaceMatrix;

float ShadowCalculation (vec4 lightSpacePos)
{
	// perform perspective divide
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	if(projCoords.z > 1.0)
		return 0.0;

	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	float bias = 0.0000001;

	// Check whether current frag pos is in shadow
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			vec3 samplePos = vec3 (projCoords.xy + vec2 (x, y) * texelSize, projCoords.z);
			shadow += texture (shadowMap, samplePos, bias);
		}    
	}

	shadow /= 9.0;

	return shadow;
}

float CalcShadowContribution (vec3 viewPosition)
{
	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrix * inverseViewMatrix * vec4 (viewPosition, 1.0f);
	float shadow = ShadowCalculation (lightSpacePos);

	return shadow;
}
