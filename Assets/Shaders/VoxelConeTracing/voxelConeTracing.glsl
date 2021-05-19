uniform sampler3D voxelTexture;
uniform sampler3D voxelMipmapTexture;

uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;
uniform int volumeMipmapLevels;

uniform float originBias;

float GetInterpolatedComp (float comp, float minValue, float maxValue)
{
	return ((comp - minValue) / (maxValue - minValue));
}

vec3 GetPositionInVolume (vec3 origin)
{
	vec3 positionInVolume;

	positionInVolume.x = GetInterpolatedComp (origin.x, minVertex.x, maxVertex.x);
	positionInVolume.y = GetInterpolatedComp (origin.y, minVertex.y, maxVertex.y);
	positionInVolume.z = GetInterpolatedComp (origin.z, minVertex.z, maxVertex.z);

	return positionInVolume;// + vec3 (1.0 / volumeSize.x);
}

float minVoxelDiameter = 1.0 / volumeSize.x;
float minVoxelDiameterInv = volumeSize.x;
const float faceCount = 6.0;
const float faceCountInv = 1.0 / faceCount;

// origin, dir, and maxDist are in texture space
// dir should be normalized
// coneRatio is the cone diameter to height ratio (2.0 for 90-degree cone)
vec4 voxelTraceCone(vec3 origin, vec3 dir, float coneRatio, float maxDist)
{
	vec3 accum = vec3(0.0);
	float alpha = 0.0;

	// the starting sample diameter
	float minDiameter = minVoxelDiameter;

	// push out the starting point to avoid self-intersection
	float startDist = minDiameter;
	
	float dist = startDist;
	while (dist <= maxDist && alpha < 1.0)
	{
		// ensure the sample diameter is no smaller than the min
		// desired diameter for this cone (ensuring we always
		// step at least minDiameter each iteration, even for tiny
		// cones - otherwise lots of overlapped samples)
		float sampleDiameter = max(minDiameter, coneRatio * dist);
		
		// convert diameter to LOD
		// for example:
		// log2(1/256 * 256) = 0
		// log2(1/128 * 256) = 1
		// log2(1/64 * 256) = 2
		float sampleLOD = log2(sampleDiameter * minVoxelDiameterInv);
		
		vec3 samplePos = origin + dir * dist;

		/*
		 * Check sampling position limits
		*/

		bvec3 a = greaterThan(samplePos, vec3(1.0));
		bvec3 b = lessThan(samplePos, vec3(0.0));

		if (any(bvec2(any(a), any(b)))) {
			break;
		}

		ivec3 face = ivec3 (
			dir.x > 0 ? 0 : 1,
			dir.y > 0 ? 2 : 3,
			dir.z > 0 ? 4 : 5
		);

		vec3 weight = dir * dir;

		/*
		 * Current mipmap level
		*/

		vec4 sampleValue = vec4 (0.0);

		vec3 mipmapSamplePos = vec3 (
			samplePos.x * faceCountInv + faceCountInv * face.x,
			samplePos.x * faceCountInv + faceCountInv * face.y,
			samplePos.x * faceCountInv + faceCountInv * face.z
		);

		if (sampleLOD < 1) {
			vec4 sampleValue1 = texture (voxelTexture, samplePos);

			/*
			 * Next mipmap level
			*/

			vec4 sampleValue2 =
				weight.x * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.x, samplePos.yz), 0) +
				weight.y * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.y, samplePos.yz), 0) +
				weight.z * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.z, samplePos.yz), 0);

			sampleValue = mix (sampleValue1, sampleValue2, fract(sampleLOD));
		} else {
			sampleLOD -= 1.0;

			sampleValue =
				weight.x * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.x, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1)) +
				weight.y * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.y, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1)) +
				weight.z * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.z, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1));
		}

		/*
		 * Mipmap filter
		*/

		accum = accum + (1.0 - alpha) * sampleValue.a * sampleValue.rgb;
		alpha = alpha + (1.0 - alpha) * sampleValue.a;

		dist += sampleDiameter;
	}
	
	// decompress color range to decode limited HDR
	// accum *= 2.0;
	
	return vec4 (accum, alpha);
}

float voxelTraceConeOcclusion(vec3 origin, vec3 dir, float coneRatio, float maxDist)
{
	float occlusion = 0.0;
	float alpha = 0.0;

	// the starting sample diameter
	float minDiameter = minVoxelDiameter;

	// push out the starting point to avoid self-intersection
	float startDist = minDiameter;
	
	float dist = startDist;
	while (dist <= maxDist && alpha < 1.0)
	{
		float sampleDiameter = max(minDiameter, coneRatio * dist);
		
		float sampleLOD = log2(sampleDiameter * minVoxelDiameterInv);
		
		vec3 samplePos = origin + dir * dist;

		/*
		 * Check sampling position limits
		*/

		bvec3 a = greaterThan(samplePos, vec3(1.0));
		bvec3 b = lessThan(samplePos, vec3(0.0));

		if (any(bvec2(any(a), any(b)))) {
			break;
		}

		ivec3 face = ivec3 (
			dir.x > 0 ? 0 : 1,
			dir.y > 0 ? 2 : 3,
			dir.z > 0 ? 4 : 5
		);

		vec3 weight = dir * dir;

		/*
		 * Current mipmap level
		*/

		float sampleValue = 0.0;

		vec3 mipmapSamplePos = vec3 (
			samplePos.x * faceCountInv + faceCountInv * face.x,
			samplePos.x * faceCountInv + faceCountInv * face.y,
			samplePos.x * faceCountInv + faceCountInv * face.z
		);

		if (sampleLOD < 1) {
			float sampleValue1 = texture (voxelTexture, samplePos).a;

			/*
			 * Next mipmap level
			*/

			// float sampleValue2 =
			// 	weight.x * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.x, samplePos.yz), 0).a +
			// 	weight.y * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.y, samplePos.yz), 0).a +
			// 	weight.z * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.z, samplePos.yz), 0).a;
			float sampleValue2 = textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.x, samplePos.yz), 0).a;

			sampleValue = mix (sampleValue1, sampleValue2, fract(sampleLOD));
		} else {
			sampleLOD -= 1.0;

			// sampleValue =
			// 	weight.x * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.x, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1)).a +
			// 	weight.y * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.y, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1)).a +
			// 	weight.z * textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.z, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1)).a;
			sampleValue = textureLod (voxelMipmapTexture, vec3 (mipmapSamplePos.x, samplePos.yz), min (sampleLOD, volumeMipmapLevels - 1)).a;
		}

		/*
		 * Mipmap filter
		*/

		occlusion += ((1.0 - alpha) * sampleValue) / (1.0 + 0.03 * sampleDiameter);

		alpha = alpha + (1.0 - alpha) * sampleValue;

		dist += sampleDiameter;
	}
	
	return min (occlusion, 1.0);
}
