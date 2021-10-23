#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define VERTEX_COUNT 3

in vec3 vert_position[];
in vec3 vert_normal[];
in vec2 vert_texcoord[];

uniform layout (binding = 0, r32i) coherent volatile iimage3D lpvVolumeR;
uniform layout (binding = 1, r32i) coherent volatile iimage3D lpvVolumeG;
uniform layout (binding = 2, r32i) coherent volatile iimage3D lpvVolumeB;

uniform layout (binding = 3, r32ui) coherent volatile uimage3D lpvGeometryVolume;

uniform layout (binding = 4, r32ui) coherent volatile uimage3D lpvSampleCountVolume;

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

uniform int vplsCount;
uniform int emissiveTextured;

out vec3 geom_worldPosition;
out vec3 geom_worldNormal;
out vec2 geom_texcoord;

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
  imageAtomicAdd(lpvSampleCountVolume, ivec3 (rsmSample.volumePos), uint(1));

}

float rand(vec3 co){
  return fract(sin(dot(co.xy + co.xz + co.yz ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 rand3(vec3 p)
{
	vec3 q = vec3(
		dot(p, vec3(127.1, 311.7, 74.7)),
		dot(p, vec3(269.5, 183.3, 246.1)),
		dot(p, vec3(113.5, 271.9, 124.6))
		);

	return fract(sin(q) * 43758.5453123);
}

void GenerateVPL (int iteration)
{
	float r1 = sqrt (rand ((vert_position [0] + vert_position [1] + vert_position [2]) + vec3 (iteration)));
	float r2 = rand ((vert_normal [0] + vert_normal [1] + vert_normal [2]) + vec3 (iteration));

	RSMSample rsmSample;

	rsmSample.worldSpacePosition =
		(1 - r1) * vert_position [0] +
		r1 * (1 - r2) * vert_position [1] +
		r1 * r2 * vert_position [2];
	rsmSample.worldSpacePosition = vec3 (modelMatrix * vec4 (rsmSample.worldSpacePosition, 1.0));

	rsmSample.worldSpaceNormal =
		(1 - r1) * vert_normal [0] +
		r1 * (1 - r2) * vert_normal [1] +
		r1 * r2 * vert_normal [2];
	// rsmSample.worldSpaceNormal = cross (
	// 	normalize (vert_position [1] - vert_position [0]),
	// 	normalize (vert_position [2] - vert_position [0])
	// );

	rsmSample.worldSpaceNormal = normalMatrix * rsmSample.worldSpaceNormal;

	// vec3 emissiveMap = MaterialEmissive;

	// if (emissiveTextured == 1) {
		vec2 texcoord =
			(1 - r1) * vert_texcoord [0] +
			r1 * (1 - r2) * vert_texcoord [1] +
			r1 * r2 * vert_texcoord [2];

		vec3 emissiveMap = MaterialEmissive * vec3 (textureLod (EmissiveMap, texcoord.xy, 0));
	// }

	rsmSample.flux.rgb = emissiveMap;
	rsmSample.volumePos = GetPositionInVolume (rsmSample.worldSpacePosition);

	GeometryInjection (rsmSample);
	RadianceInjection (rsmSample);
}

void main ()
{
	for (int i = 0; i < vplsCount; i++) {
		GenerateVPL (i);
	}

	EndPrimitive ();

}
