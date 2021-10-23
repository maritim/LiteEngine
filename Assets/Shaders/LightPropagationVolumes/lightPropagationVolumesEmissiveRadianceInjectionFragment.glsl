#version 450 core

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

uniform float normalAngleStep;
uniform int emissiveTextured;

in vec3 geom_worldPosition;
in vec3 geom_worldNormal;
in vec2 geom_texcoord;
in mat3 geom_swizzleMatrixInv;
in vec4 geom_BBox;
in vec3 geom_initialScreenPos[3];
in vec3 geom_vertexWorldPos[3];
in vec3 geom_vertexWorldNormal[3];
in vec2 geom_vertexTexcoord[3];
// in float geom_texLod;

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

void RadianceInjection (const in RSMSample rsmSample, const in uint count)
{
	vec4 shCoefficients = evalCosineLobeToDir (rsmSample.worldSpaceNormal) / PI;

	imgStoreAdd(lpvVolumeR, ivec3 (rsmSample.volumePos), (shCoefficients * rsmSample.flux.r));
	imgStoreAdd(lpvVolumeG, ivec3 (rsmSample.volumePos), (shCoefficients * rsmSample.flux.g));
	imgStoreAdd(lpvVolumeB, ivec3 (rsmSample.volumePos), (shCoefficients * rsmSample.flux.b));
  imageAtomicAdd(lpvSampleCountVolume, ivec3 (rsmSample.volumePos), count);
}

float sign (const in vec2 p1, const in vec2 p2, const in vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (const in vec2 s, const in vec2 triangle [3])
{
    float as_x = s.x-triangle [0].x;
    float as_y = s.y-triangle [0].y;

    bool s_ab = (triangle [1].x-triangle [0].x)*as_y-(triangle [1].y-triangle [0].y)*as_x > 0;

    if((triangle [2].x-triangle [0].x)*as_y-(triangle [2].y-triangle [0].y)*as_x > 0 == s_ab) return false;

    if((triangle [2].x-triangle [1].x)*(s.y-triangle [1].y)-(triangle [2].y-triangle [1].y)*(s.x-triangle [1].x) > 0 != s_ab) return false;

    return true;

    // float d1, d2, d3;
    // bool has_neg, has_pos;

    // d1 = sign(pt, triangle [0], triangle [1]);
    // d2 = sign(pt, triangle [1], triangle [2]);
    // d3 = sign(pt, triangle [2], triangle [0]);

    // has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    // has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    // return !(has_neg && has_pos);
}

#include "LightPropagationVolumes/triangleBoxIntersection.glsl"

// float GetTextureLod (vec2 dx_texcoord, vec2 dy_texcoord)
// {
// 	ivec2 texSize = textureSize (EmissiveMap, 0);

// 	vec2 dx = dx_texcoord * texSize;
// 	vec2 dy = dy_texcoord * texSize;
// 	float d = max (dot (dx, dx), dot (dy, dy));

// 	return 0.5 * log2 (d);
// }

vec3 GetWeights (const in vec2 point, const in vec2 triangle [3])
{
	// vec3 weights;

	// mat2 T = mat2 (
	// 	triangle [0].x - triangle [2].x,
	// 	triangle [0].y - triangle [2].y,
	// 	triangle [1].x - triangle [2].x,
	// 	triangle [1].y - triangle [2].y
	// );

	// weights.xy = inverse (T) * (point - triangle [2]);

	// weights.z = 1.0 - weights.x - weights.y;

	// return weights;

	float w1 =
		((triangle [1].y - triangle [2].y) * (point.x - triangle [2].x) +
			(triangle [2].x - triangle [1].x) * (point.y - triangle [2].y)) /
		((triangle [1].y - triangle [2].y) * (triangle [0].x - triangle [2].x) +
			(triangle [2].x - triangle [1].x) * (triangle [0].y - triangle [2].y));

	float w2 =
		((triangle [2].y - triangle [0].y) * (point.x - triangle [2].x) +
			(triangle [0].x - triangle [2].x) * (point.y - triangle [2].y)) /
		((triangle [1].y - triangle [2].y) * (triangle [0].x - triangle [2].x) +
			(triangle [2].x - triangle [1].x) * (triangle [0].y - triangle [2].y));

	float w3 = 1.0 - w1 - w2;

	return vec3 (w1, w2, w3);
}

// vec4 GetIntersectionBBox (const in vec4 bbox, const in vec4 triangleBBox)
// {
// 	return vec4 (
// 		max (bbox.xy, triangleBBox.xy),
// 		min (bbox.zw, triangleBBox.zw)
// 	);
// }

bool Intersects(vec2 a1, vec2 a2, vec2 b1, vec2 b2, out vec2 intersectionPoint)
{
    vec2 b = a2 - a1;
    vec2 d = b2 - b1;
    float bDotDPerp = b.x * d.y - b.y * d.x;

    // if b dot d == 0, it means the lines are parallel so have infinite intersection points
    if (bDotDPerp == 0)
        return false;

    vec2 c = b1 - a1;
    float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
    if (t < 0 || t > 1)
        return false;

    float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
    if (u < 0 || u > 1)
        return false;

    intersectionPoint = a1 + t * b;

    return true;
}

vec4 GetIntersectionBBox (const in vec4 bbox, const in vec2 triangle [3])
{
	vec4 intersectionBBox = vec4 (vec2 (1.0 / 0.0), vec2 (-1.0 / 0.0));

	int intersectionCornerCount = 0;
	vec2 intersectionCorner [10];

	for (int vertexIndex = 0; vertexIndex < 3; vertexIndex ++) {
		if (all(greaterThanEqual(triangle [vertexIndex], bbox.xy)) &&
				all(lessThanEqual(triangle [vertexIndex], bbox.zw))) {

			intersectionCorner [intersectionCornerCount++] = triangle [vertexIndex];
		}
	}

	vec2 bboxCorner [4] = {
		bbox.xy, bbox.zy, bbox.zw, bbox.xw 
	};

	for (int bboxCornerIndex = 0; bboxCornerIndex < 4; bboxCornerIndex ++) {
		const int i = bboxCornerIndex;
		if (PointInTriangle (bboxCorner [i], triangle)) {
			intersectionCorner [intersectionCornerCount ++] = bboxCorner [i];
		}
	}

	vec2 intersectionPoint;

	if (Intersects (bboxCorner [0], bboxCorner [1], triangle [0], triangle [1], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [0], bboxCorner [1], triangle [1], triangle [2], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [0], bboxCorner [1], triangle [2], triangle [0], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [1], bboxCorner [2], triangle [0], triangle [1], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [1], bboxCorner [2], triangle [1], triangle [2], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [1], bboxCorner [2], triangle [2], triangle [0], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [2], bboxCorner [3], triangle [0], triangle [1], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [2], bboxCorner [3], triangle [1], triangle [2], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [2], bboxCorner [3], triangle [2], triangle [0], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [3], bboxCorner [0], triangle [0], triangle [1], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [3], bboxCorner [0], triangle [1], triangle [2], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	if (Intersects (bboxCorner [3], bboxCorner [0], triangle [2], triangle [0], intersectionPoint)) {
		intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	}

	// for (int bboxCornerIndex = 0; bboxCornerIndex < 4; bboxCornerIndex ++) {
	// 	const int i1 = bboxCornerIndex;
	// 	const int i2 = i1 == 3 ? 0 : (i1 + 1);
	// 	for (int vertexIndex = 0; vertexIndex < 3; vertexIndex ++) {
	// 		const int j1 = vertexIndex;
	// 		const int j2 = j1 == 2 ? 0 : (j1 + 1);
	// 		if (Intersects (
	// 			bboxCorner [0],
	// 			bboxCorner [1],
	// 			triangle [0],
	// 			triangle [1],
	// 			intersectionPoint
	// 		)) {
	// 			return vec4 (1.0);
	// 			intersectionCorner [intersectionCornerCount++] = intersectionPoint;
	// 		}
	// 	}
	// }

	for (int cornerIndex = 0; cornerIndex < intersectionCornerCount; cornerIndex ++) {
		intersectionBBox.xy = min (intersectionBBox.xy, intersectionCorner [cornerIndex]);
		intersectionBBox.zw = max (intersectionBBox.zw, intersectionCorner [cornerIndex]);
	}

	return intersectionBBox;
}

void Sample3 (const in int intersectedVoxelCount,
	const in vec3 intersectedVoxels [3])
{
	vec3 emissiveMap = MaterialEmissive;

	vec3 worldNormal = geom_worldNormal;

	if (emissiveTextured == 1) {

		vec4 pixelBBox = vec4 (
			gl_FragCoord.xy - vec2 (0.5),
			gl_FragCoord.xy + vec2 (0.5)
		);

		vec2 triangle [3] = {
			geom_initialScreenPos [0].xy,
			geom_initialScreenPos [1].xy,
			geom_initialScreenPos [2].xy
		};

		vec4 intersectionBBox = GetIntersectionBBox (pixelBBox, triangle);

		// return;
		if (intersectionBBox.x > 100 || intersectionBBox.z < 0) {
			return;
		}

		vec2 centerBBox = (intersectionBBox.xy + intersectionBBox.zw) / 2.0;

		vec3 weights = GetWeights (centerBBox, triangle);
		vec2 texcoord =
			weights.x * geom_vertexTexcoord [0] +
			weights.y * geom_vertexTexcoord [1] +
			weights.z * geom_vertexTexcoord [2];

		vec3 worldNormal =
			weights.x * geom_vertexWorldNormal [0] +
			weights.y * geom_vertexWorldNormal [1] +
			weights.z * geom_vertexWorldNormal [2];

		/*
		 * Compute corner weights
		*/

		vec3 cornerWeights [2] = {
			GetWeights (intersectionBBox.xy, triangle),
			GetWeights (intersectionBBox.zw, triangle) //,
			// GetWeights (intersetionBBox.xw, triangle),
			// GetWeights (intersetionBBox.zy, triangle)
		};

		/*
		 * Compute texture lod
		*/

		vec2 texcoordMin =
			cornerWeights [0].x * geom_vertexTexcoord [0] +
			cornerWeights [0].y * geom_vertexTexcoord [1] +
			cornerWeights [0].z * geom_vertexTexcoord [2];

		vec2 texcoordMax =
			cornerWeights [1].x * geom_vertexTexcoord [0] +
			cornerWeights [1].y * geom_vertexTexcoord [1] +
			cornerWeights [1].z * geom_vertexTexcoord [2];

		float texLod = 0.5 * log2 (dot (texcoordMin, texcoordMax));

		/*
		 * Compute color
		*/

		emissiveMap = emissiveMap * vec3 (textureLod (EmissiveMap, texcoord, 0));
	}

	/*
	 * Inject radiance
	*/

	RSMSample rsmSample = RSMSample (geom_worldPosition, worldNormal, emissiveMap, vec3 (0.0));

	for (int voxelIndex = 0; voxelIndex < intersectedVoxelCount; voxelIndex ++) {

		rsmSample.volumePos = intersectedVoxels [voxelIndex];
		rsmSample.volumePos = geom_swizzleMatrixInv * rsmSample.volumePos;

		RadianceInjection (rsmSample, 1);
		GeometryInjection (rsmSample);
	}
}

void IntersectedVoxels (out int count, out vec3 intersectedVoxel [3])
{
	count = 0;

	vec3 voxelCenter = vec3(gl_FragCoord.xy, gl_FragCoord.z * volumeSize.z);
	voxelCenter.z = floor (voxelCenter.z) + 0.5;

	for (int depth = -1; depth < 2; depth ++) {
		vec3 voxelCenter1 = vec3 (
			voxelCenter.xy,
			voxelCenter.z + depth
		);

		if (triBoxOverlap (voxelCenter1, vec3 (0.5), geom_initialScreenPos [0],
			geom_initialScreenPos [1], geom_initialScreenPos [2])) {

			intersectedVoxel [count] = voxelCenter1;
			count ++;
		}
	}
}

vec3 GetWeights3D (const in vec3 point, const in vec3 triangle [3])
{
	vec3 f1 = triangle [0] - point;
	vec3 f2 = triangle [1] - point;
	vec3 f3 = triangle [2] - point;

	float a = length (cross (triangle [0] - triangle [1], triangle [0] - triangle [2]));

	return vec3 (
		length (cross (f2, f3)) / a,
		length (cross (f3, f1)) / a,
		length (cross (f1, f2)) / a
	);
}

void Sample ()
{
	vec3 dx_normal = dFdx (geom_worldNormal);
	vec3 dy_normal = dFdy (geom_worldNormal);

	vec3 dx_worldPos = dFdx (geom_worldPosition);
	vec3 dy_worldPos = dFdy (geom_worldPosition);

	// vec3 triangle3D [3] = {
	// 	geom_vertexWorldPos [0],
	// 	geom_vertexWorldPos [1],
	// 	geom_vertexWorldPos [2]
	// };

	vec2 triangle [3] = {
		geom_initialScreenPos [0].xy,
		geom_initialScreenPos [1].xy,
		geom_initialScreenPos [2].xy
	};

	vec3 worldNormal = normalize (geom_worldNormal);
	vec3 worldNormalUpRight = normalize (geom_worldNormal + dx_normal);
	vec3 worldNormalDownLeft = normalize (geom_worldNormal + dy_normal);

	float dx = dot (worldNormalUpRight, worldNormal);
	float dy = dot (worldNormalDownLeft, worldNormal);

	int normalStepsX = 50;//max (1, int (ceil (acos (dx) / normalAngleStep)));
	int normalStepsY = 50;//max (1, int (ceil (acos (dy) / normalAngleStep)));

	// vec4 sphericalCoeff [3];

	// RSMSample rsmSample = RSMSample (geom_worldPosition, geom_worldNormal, vec3 (0), coords);
	uint counter = 0;

	for (int indexX = 0; indexX < normalStepsX; indexX ++) {
		for (int indexY = 0; indexY < normalStepsY; indexY ++) {

			float tX = (1.0 / normalStepsX) * indexX - 0.5;
			float tY = (1.0 / normalStepsY) * indexY - 0.5;

			vec2 screenPos = gl_FragCoord.xy + vec2 (tX, tY);

			// if (!PointInTriangle (screenPos, triangle)) {
			// 	continue;
			// }

			// vec3 worldPos = geom_worldPosition +
			// 	tX * dx_worldPos + tY * dy_worldPos;

			// vec3 weights = GetWeights3D (worldPos, triangle3D);
			vec3 weights = GetWeights (screenPos, triangle);

			bvec3 a = greaterThan(weights, vec3(1.0));
			bvec3 b = lessThan(weights, vec3(0.0));

			if (any(bvec2(any(a), any(b)))) {
				continue;
			}

			vec2 texcoord =
				weights.x * geom_vertexTexcoord [0] +
				weights.y * geom_vertexTexcoord [1] +
				weights.z * geom_vertexTexcoord [2];

			vec3 worldPos =
				weights.x * geom_vertexWorldPos [0] +
				weights.y * geom_vertexWorldPos [1] +
				weights.z * geom_vertexWorldPos [2];

			vec3 normal =
				weights.x * geom_vertexWorldNormal [0] +
				weights.y * geom_vertexWorldNormal [1] +
				weights.z * geom_vertexWorldNormal [2];

			vec3 volumePos = GetPositionInVolume (worldPos);

			vec3 emissiveMap = MaterialEmissive * vec3 (textureLod (EmissiveMap, texcoord, 0));

			RSMSample rsmSample = RSMSample (worldPos, normal, emissiveMap, volumePos);

			RadianceInjection (rsmSample, 1);
			GeometryInjection (rsmSample);

			// rsmSample.flux += emissiveMap * 10;

			// counter ++;
		}
	}

	// RSMSample rsmSample = RSMSample (geom_worldPosition, geom_worldNormal, vec3 (0), coords);

	// RadianceInjection (rsmSample, counter);
	// GeometryInjection (rsmSample);
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

	vec3 intersectedVoxels [3];
	int intersectedVoxelCount = 0;

	IntersectedVoxels (intersectedVoxelCount, intersectedVoxels);

	if (intersectedVoxelCount == 0) {
		discard;
	}

	Sample3 (intersectedVoxelCount, intersectedVoxels);
	// Sample ();
}
