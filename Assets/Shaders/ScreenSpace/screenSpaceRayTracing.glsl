vec2 ScreenSpaceRayTrace (vec3 startPosition, vec3 rayDirection, sampler2D viewPositionMap,
	int maxIterations, float sampleSkip, float spatialBias)
{
	vec2 reflectionPos = vec2 (0.0f);

	int iterations = 0;

	bool needStop = false;

	while (iterations < maxIterations && needStop == false) {

		vec3 rayPos = startPosition + iterations * rayDirection * sampleSkip;

		vec4 projectedPos = projectionMatrix * vec4 (rayPos, 1.0f);
		projectedPos.xyz /= projectedPos.w;
		projectedPos.xyz = projectedPos.xyz * 0.5 + 0.5;

		if (projectedPos.x > 1 || projectedPos.x < 0 || projectedPos.y > 1 || projectedPos.y < 0) {
			needStop = true;
		}

		vec3 samplePos = texture (viewPositionMap, projectedPos.xy).xyz;

		if (rayPos.z < samplePos.z && rayPos.z > samplePos.z - spatialBias) {
			needStop = true;

			reflectionPos = projectedPos.xy;
		}

		iterations ++;
	}

	return reflectionPos;
}
