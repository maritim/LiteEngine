#version 420 core

uniform layout (binding = 0, r32i) coherent volatile iimage3D lpvVolumeR;
uniform layout (binding = 1, r32i) coherent volatile iimage3D lpvVolumeG;
uniform layout (binding = 2, r32i) coherent volatile iimage3D lpvVolumeB;

uniform layout (binding = 3, r32ui) coherent volatile uimage3D lpvGeometryVolume;

#define SH_F2I 1000.0

#define imgStoreAdd(img, pos, data) \
  imageAtomicAdd(img, ivec3(pos.x * 4, pos.y, pos.z), int(data.x * SH_F2I)); \
  imageAtomicAdd(img, ivec3(pos.x * 4 + 1, pos.y, pos.z), int(data.y * SH_F2I)); \
  imageAtomicAdd(img, ivec3(pos.x * 4 + 2, pos.y, pos.z), int(data.z * SH_F2I)); \
  imageAtomicAdd(img, ivec3(pos.x * 4 + 3, pos.y, pos.z), int(data.w * SH_F2I));

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;

uniform vec3 MaterialEmissive;

uniform sampler2D EmissiveMap;

uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;

in vec3 geom_worldPosition;
in vec3 geom_worldNormal;
in vec2 geom_texcoord;
in mat3 geom_swizzleMatrixInv;
in vec4 geom_BBox;

struct RSMSample
{
	vec3 worldSpacePosition;
	vec3 worldSpaceNormal;
	vec3 flux;
	vec3 volumePos;
};

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

	return positionInVolume * volumeSize.x;// + vec3 (1.0);
}

#define PI 3.1415926f

/*Spherical harmonics coefficients - precomputed*/
#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2

/*Cosine lobe coeff*/
#define SH_cosLobe_C0 0.886226925f // sqrt(pi)/2 
#define SH_cosLobe_C1 1.02332671f // sqrt(pi/3) 

vec4 evalCosineLobeToDir(vec3 dir) {
	dir = normalize(dir);
	//f00, f-11, f01, f11
	return vec4( SH_cosLobe_C0, -SH_cosLobe_C1 * dir.y, SH_cosLobe_C1 * dir.z, -SH_cosLobe_C1 * dir.x );
}

// float calculateSurfelAreaLightOrtho(vec3 lightPos) {
// 	float rsmSize = (rsmResolution / 4).x;

// 	lightPos = lightPos * 2.0 - 1.0;

// 	return (4.0 * lightPos.z * lightPos.z)/(rsmSize * rsmSize);
// }

//(As * clamp(dot(ns,w),0.0,1.0))/(cellsize * cellsize)
// float calculateBlockingProbability(float surfelArea, vec3 normal)
// {
// 	float volumeCellSize = (maxVertex.x - minVertex.x) / volumeSize.x;

// 	return clamp((surfelArea * clamp(dot(normal, -lightDirection),0.0,1.0))/(volumeCellSize*volumeCellSize),0.0,1.0); //It is probability so 0.0 - 1.0
// }

// void CalcBias (inout RSMSample rsmSample)
// {
// 	float volumeCellSize = (maxVertex.x - minVertex.x) / volumeSize.x;

// 	float bias = volumeCellSize * injectionBias;

// 	rsmSample.worldSpacePosition += rsmSample.worldSpaceNormal * bias;
// 	rsmSample.worldSpacePosition += -lightDirection * bias;

// 	rsmSample.volumePos = GetPositionInVolume (rsmSample.worldSpacePosition);
// }

void GeometryInjection (in RSMSample rsmSample)
{
	// vec3 lightPos = vec3 (lightSpaceMatrix * vec4 (rsmSample.worldSpacePosition, 1.0));

	// float surfelArea = calculateSurfelAreaLightOrtho (lightPos);

	float blockingProbability = 1.0f;// calculateBlockingProbability(1, rsmSample.worldSpaceNormal);

	vec4 shCoefficients = evalCosineLobeToDir(rsmSample.worldSpaceNormal);// * blockingProbability;

	imageAtomicMax(lpvGeometryVolume, ivec3(rsmSample.volumePos), packUnorm4x8 (shCoefficients));
}

void RadianceInjection (in RSMSample rsmSample)
{
	vec4 shCoefficients = evalCosineLobeToDir (rsmSample.worldSpaceNormal) / PI;

	imgStoreAdd(lpvVolumeR, ivec3 (rsmSample.volumePos), (shCoefficients * rsmSample.flux.r));
	imgStoreAdd(lpvVolumeG, ivec3 (rsmSample.volumePos), (shCoefficients * rsmSample.flux.g));
	imgStoreAdd(lpvVolumeB, ivec3 (rsmSample.volumePos), (shCoefficients * rsmSample.flux.b));

}

void main ()
{
	/*
	 *
	*/

	vec2 bboxMin = floor((geom_BBox.xy * 0.5 + 0.5) * volumeSize.xy);
	vec2 bboxMax = ceil((geom_BBox.zw * 0.5 + 0.5) * volumeSize.xy);
	if (!all(greaterThanEqual(gl_FragCoord.xy, bboxMin)) || !all(lessThanEqual(gl_FragCoord.xy, bboxMax))) {
		discard;
	}

	/*
	 * Calculate the position in texture 3D
	*/
	
	vec3 coords = geom_swizzleMatrixInv * vec3(gl_FragCoord.xy, gl_FragCoord.z * volumeSize.z);

	/*
	 * Get color of all used texture maps
	*/

	vec3 emissiveMap = MaterialEmissive * vec3 (texture2D (EmissiveMap, geom_texcoord.xy));

	RSMSample rsmSample = RSMSample (geom_worldPosition, geom_worldNormal, emissiveMap * 10, coords);

	// CalcBias (rsmSample);

	GeometryInjection (rsmSample);
	RadianceInjection (rsmSample);
}
