#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define VERTEX_COUNT 3

in vec3 vert_worldPosition[];
in vec3 vert_worldNormal[];
in vec2 vert_texcoord[];

out vec3 geom_worldPosition;
out vec3 geom_worldNormal;
out vec2 geom_texcoord;
out mat3 geom_swizzleMatrixInv;
out vec4 geom_BBox;
out vec3 geom_initialScreenPos[3];
out vec3 geom_vertexWorldPos[3];
out vec3 geom_vertexWorldNormal[3];
out vec2 geom_vertexTexcoord[3];
out float geom_texLod;

uniform vec3 minVertex;
uniform vec3 maxVertex;

uniform sampler2D EmissiveMap;

uniform ivec3 volumeSize;

mat3 GetSwizzleMatrix ()
{
	vec3 worldSpaceV1 = vert_worldPosition[1].xyz - vert_worldPosition[0].xyz;
	vec3 worldSpaceV2 = vert_worldPosition[2].xyz - vert_worldPosition[0].xyz;
	vec3 worldSpaceNormal = abs(cross(worldSpaceV1, worldSpaceV2));

	if (worldSpaceNormal.x >= worldSpaceNormal.y && worldSpaceNormal.x >= worldSpaceNormal.z) {
		return mat3(vec3(0.0, 0.0, 1.0),
					vec3(0.0, 1.0, 0.0),
					vec3(1.0, 0.0, 0.0));
	}
	else if (worldSpaceNormal.y >= worldSpaceNormal.z) {
		return mat3(vec3(1.0, 0.0, 0.0),
					vec3(0.0, 0.0, 1.0),
					vec3(0.0, 1.0, 0.0));
	}

	return mat3(vec3(1.0, 0.0, 0.0),
				vec3(0.0, 1.0, 0.0),
				vec3(0.0, 0.0, 1.0));
}

float GetPixelSize ()
{
	return 1.0 / volumeSize.x;
}

float GetPixelDiagonal ()
{
	float pixelSize = GetPixelSize ();

	return 1.41421f * pixelSize;
}

 void ExpandTriangle(inout vec4 screenPos[3], float pixelDiagonal)
 {
	vec2 edge[3];
	edge[0] = screenPos[1].xy - screenPos[0].xy;
	edge[1] = screenPos[2].xy - screenPos[1].xy;
	edge[2] = screenPos[0].xy - screenPos[2].xy;

	vec2 edgeNormal[3];
	edgeNormal[0] = normalize(edge[0]);
	edgeNormal[1] = normalize(edge[1]);
	edgeNormal[2] = normalize(edge[2]);
	edgeNormal[0] = vec2(-edgeNormal[0].y, edgeNormal[0].x);
	edgeNormal[1] = vec2(-edgeNormal[1].y, edgeNormal[1].x);
	edgeNormal[2] = vec2(-edgeNormal[2].y, edgeNormal[2].x);

	// If triangle is back facing, flip it's edge normals so triangle does not shrink.
	vec3 a = normalize(screenPos[1].xyz - screenPos[0].xyz);
	vec3 b = normalize(screenPos[2].xyz - screenPos[0].xyz);
	vec3 clipSpaceNormal = cross(a, b);
	if (clipSpaceNormal.z < 0.0) {
		edgeNormal[0] *= -1.0;
		edgeNormal[1] *= -1.0;
		edgeNormal[2] *= -1.0;
	}

	vec3 edgeDist;
	edgeDist.x = dot(edgeNormal[0], screenPos[0].xy);
	edgeDist.y = dot(edgeNormal[1], screenPos[1].xy);
	edgeDist.z = dot(edgeNormal[2], screenPos[2].xy);

	screenPos[0].xy = screenPos[0].xy - pixelDiagonal * (edge[2] / dot(edge[2], edgeNormal[0]) + edge[0] / dot(edge[0], edgeNormal[2]));
	screenPos[1].xy = screenPos[1].xy - pixelDiagonal * (edge[0] / dot(edge[0], edgeNormal[1]) + edge[1] / dot(edge[1], edgeNormal[0]));
	screenPos[2].xy = screenPos[2].xy - pixelDiagonal * (edge[1] / dot(edge[1], edgeNormal[2]) + edge[2] / dot(edge[2], edgeNormal[1]));
}

float GetInterpolatedComp (float comp, float minValue, float maxValue)
{
	return ((comp - minValue) / (maxValue - minValue));
}

vec4 GetScreenNormalizedPosition (vec4 position)
{
	vec4 screenPos = vec4 (1.0);

	screenPos.x = GetInterpolatedComp (position.x, minVertex.x, maxVertex.x);
	screenPos.y = GetInterpolatedComp (position.y, minVertex.y, maxVertex.y);
	screenPos.z = GetInterpolatedComp (position.z, minVertex.z, maxVertex.z);

	screenPos.xyz = (vec3 (screenPos) * vec3 (2.0)) - vec3 (1.0);

	return screenPos;
}

void main ()
{
    /*
	 * Calculate swizzle matrix based on world space normal's dominant direction.
	*/

	mat3 swizzleMatrix = GetSwizzleMatrix ();

	geom_swizzleMatrixInv = inverse (swizzleMatrix);

    /*
	 * Expand triangle for conservative rasterization
	*/

	vec4 screenPos [VERTEX_COUNT];

	for (int index = 0;index < VERTEX_COUNT; ++index) {
		screenPos [index] = vec4 (vert_worldPosition [index], 1.0);
		screenPos [index] = GetScreenNormalizedPosition (screenPos [index]);
		screenPos [index] = vec4 (swizzleMatrix * screenPos [index].xyz, 1.0f);
	}

	float pixelDiagonal = GetPixelDiagonal ();

	// Calculate screen space bounding box to be used for clipping in the fragment shader.
	vec4 screenBBox;
    screenBBox.xy = min(screenPos[0].xy, min(screenPos[1].xy, screenPos[2].xy));
    screenBBox.zw = max(screenPos[0].xy, max(screenPos[1].xy, screenPos[2].xy));
    geom_BBox.xy = screenBBox.xy - vec2(pixelDiagonal);
    geom_BBox.zw = screenBBox.zw + vec2(pixelDiagonal);

    geom_initialScreenPos [0] = (screenPos [0].xyz * 0.5 + 0.5) * volumeSize;
    geom_initialScreenPos [1] = (screenPos [1].xyz * 0.5 + 0.5) * volumeSize;
    geom_initialScreenPos [2] = (screenPos [2].xyz * 0.5 + 0.5) * volumeSize;

	ExpandTriangle(screenPos, pixelDiagonal);

	/*
	 *
	*/

	geom_vertexWorldPos [0] = vert_worldPosition [0];
	geom_vertexWorldPos [1] = vert_worldPosition [1];
	geom_vertexWorldPos [2] = vert_worldPosition [2];

	/*
	 * Compute normal vectors angle difference per pixel
	*/

	geom_vertexWorldNormal [0] = vert_worldNormal [0];
	geom_vertexWorldNormal [1] = vert_worldNormal [1];
	geom_vertexWorldNormal [2] = vert_worldNormal [2];

	/*
	 *
	*/

	geom_vertexTexcoord [0] = vert_texcoord [0];
	geom_vertexTexcoord [1] = vert_texcoord [1];
	geom_vertexTexcoord [2] = vert_texcoord [2];

	/*
	 * Emit every vertex of the triangle, one by one
	*/

	for (int index = 0; index < VERTEX_COUNT; ++index) {
		gl_Position = screenPos [index];

		geom_worldPosition = vert_worldPosition [index];
		geom_worldNormal = vert_worldNormal [index];
		geom_texcoord = vert_texcoord [index];

		EmitVertex ();
	}

	EndPrimitive ();
}
