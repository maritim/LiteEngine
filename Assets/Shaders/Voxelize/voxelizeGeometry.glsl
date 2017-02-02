#version 420 core

#extension GL_ARB_shader_image_size : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define VERTEX_COUNT 3

#define PIXEL_DIAGONAL 2000

in vec3 vert_worldPosition[];
in vec3 vert_worldNormal[];
in vec2 vert_texcoord[];

out vec3 geom_worldPosition;
out vec3 geom_worldNormal;
out vec2 geom_texcoord;
out mat3 geom_swizzleMatrixInv;

layout (binding = 0, rgba8) coherent uniform image3D volumeTexture;

void main ()
{
    // Calculate swizzle matrix based on eye space normal's dominant direction.
    vec3 eyeSpaceV1 = normalize(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);
	vec3 eyeSpaceV2 = normalize(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
	vec3 eyeSpaceNormal = abs(cross(eyeSpaceV1, eyeSpaceV2));
	float dominantAxis = max(eyeSpaceNormal.x, max(eyeSpaceNormal.y, eyeSpaceNormal.z));

	mat3 swizzleMatrix;
	if (dominantAxis == eyeSpaceNormal.x)
    {
		swizzleMatrix = mat3(vec3(0.0, 0.0, 1.0),
							 vec3(0.0, 1.0, 0.0),
							 vec3(1.0, 0.0, 0.0));
    }
	else if (dominantAxis == eyeSpaceNormal.y)
    {
		swizzleMatrix = mat3(vec3(1.0, 0.0, 0.0),
						 	 vec3(0.0, 0.0, 1.0),
							 vec3(0.0, 1.0, 0.0));
    }
	else if (dominantAxis == eyeSpaceNormal.z)
    {
		swizzleMatrix = mat3(vec3(1.0, 0.0, 0.0),
							 vec3(0.0, 1.0, 0.0),
							 vec3(0.0, 0.0, 1.0));
    }

    // Pass inverse of swizzle matrix to fragment shader.
    geom_swizzleMatrixInv = inverse(swizzleMatrix);

 //    // Calculate screen coordinates for triangle.
	// vec4 screenPos[3];
	// screenPos[0] = ProjectionMatrix * vec4(swizzleMatrix * gl_in[0].gl_Position.xyz, 1.0);
	// screenPos[1] = ProjectionMatrix * vec4(swizzleMatrix * gl_in[1].gl_Position.xyz, 1.0);
	// screenPos[2] = ProjectionMatrix * vec4(swizzleMatrix * gl_in[2].gl_Position.xyz, 1.0);
 //    screenPos[0] /= screenPos[0].w;
 //    screenPos[1] /= screenPos[1].w;
 //    screenPos[2] /= screenPos[2].w;
    
 //    // Calculate screen space bounding box to be used for clipping in the fragment shader.
 //    geom_BBox.xy = min(screenPos[0].xy, min(screenPos[1].xy, screenPos[2].xy));
 //    geom_BBox.zw = max(screenPos[0].xy, max(screenPos[1].xy, screenPos[2].xy));
 //    geom_BBox.xy -= vec2(PIXEL_DIAGONAL);
 //    geom_BBox.zw += vec2(PIXEL_DIAGONAL);

    // // Expand triangle for conservative rasterization.
    // if (ConservativeRasterization)
    // {
    //     expandTriangle(screenPos);
    // }

	/*
	 * Emit every vertex of the triangle, ony by one
	*/

	for (int index = 0; index < VERTEX_COUNT; ++index) {
		gl_Position = gl_in [index].gl_Position;

		geom_worldPosition = vert_worldPosition [index];
		geom_worldNormal = vert_worldNormal [index];
		geom_texcoord = vert_texcoord [index];

		EmitVertex ();
	}

	EndPrimitive ();
}

// void expandTriangle(inout vec4 screenPos[3])
// {
// 	vec2 edge[3];
// 	edge[0] = screenPos[1].xy - screenPos[0].xy;
// 	edge[1] = screenPos[2].xy - screenPos[1].xy;
// 	edge[2] = screenPos[0].xy - screenPos[2].xy;

// 	vec2 edgeNormal[3];
// 	edgeNormal[0] = normalize(edge[0]);
// 	edgeNormal[1] = normalize(edge[1]);
// 	edgeNormal[2] = normalize(edge[2]);
// 	edgeNormal[0] = vec2(-edgeNormal[0].y, edgeNormal[0].x);
// 	edgeNormal[1] = vec2(-edgeNormal[1].y, edgeNormal[1].x);
// 	edgeNormal[2] = vec2(-edgeNormal[2].y, edgeNormal[2].x);

//     // If triangle is back facing, flip it's edge normals so triangle does not shrink.
//     vec3 a = normalize(screenPos[1].xyz - screenPos[0].xyz);
// 	vec3 b = normalize(screenPos[2].xyz - screenPos[0].xyz);
// 	vec3 clipSpaceNormal = cross(a, b);
//     if (clipSpaceNormal.z < 0.0)
//     {
//         edgeNormal[0] *= -1.0;
//         edgeNormal[1] *= -1.0;
//         edgeNormal[2] *= -1.0;
//     }

// 	vec3 edgeDist;
// 	edgeDist.x = dot(edgeNormal[0], screenPos[0].xy);
// 	edgeDist.y = dot(edgeNormal[1], screenPos[1].xy);
// 	edgeDist.z = dot(edgeNormal[2], screenPos[2].xy);

// 	screenPos[0].xy = screenPos[0].xy - PixelDiagonal * (edge[2] / dot(edge[2], edgeNormal[0]) + edge[0] / dot(edge[0], edgeNormal[2]));
// 	screenPos[1].xy = screenPos[1].xy - PixelDiagonal * (edge[0] / dot(edge[0], edgeNormal[1]) + edge[1] / dot(edge[1], edgeNormal[0]));
// 	screenPos[2].xy = screenPos[2].xy - PixelDiagonal * (edge[1] / dot(edge[1], edgeNormal[2]) + edge[2] / dot(edge[2], edgeNormal[1]));
// }
