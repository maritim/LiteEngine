#version 420 core

uniform layout (binding = 3, r32ui) coherent volatile uimage3D lpvGeometryVolume;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;
uniform mat3 inverseNormalWorldMatrix;

uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;

uniform vec3 lightDirection;

#include "deferred.glsl"

vec2 CalcTexCoordRSM()
{
	return gl_FragCoord.xy / (screenSize / 4);
}

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
float calculateBlockingProbability(float surfelArea, vec3 normal)
{
	float volumeCellSize = (maxVertex.x - minVertex.x) / volumeSize.x;

	return clamp((surfelArea * clamp(dot(normal, -lightDirection),0.0,1.0))/(volumeCellSize*volumeCellSize),0.0,1.0); //It is probability so 0.0 - 1.0
}

void GeometryInjection (vec3 in_position, vec3 in_normal)
{
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 volumePos = GetPositionInVolume (worldPosition);

	float blockingProbability = calculateBlockingProbability(1, worldNormal);

	vec4 shCoefficients = evalCosineLobeToDir(worldNormal);// * blockingProbability;

	imageAtomicMax(lpvGeometryVolume, ivec3(volumePos), packUnorm4x8 (shCoefficients));
}

void main ()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	GeometryInjection (in_position, in_normal);
}
