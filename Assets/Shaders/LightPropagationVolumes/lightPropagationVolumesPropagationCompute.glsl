#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

uniform sampler3D volumeTextureR;
uniform sampler3D volumeTextureG;
uniform sampler3D volumeTextureB;
uniform ivec3 volumeSize;
uniform int iteration;
uniform int geometryOcclusion;
// uniform float occlusionIntensity;

uniform sampler3D geometryTexture;

layout(binding = 0, rgba32f) uniform writeonly image3D lpvVolumeR;
layout(binding = 1, rgba32f) uniform writeonly image3D lpvVolumeG;
layout(binding = 2, rgba32f) uniform writeonly image3D lpvVolumeB;

layout(binding = 3, rgba32f) uniform image3D lpvAccumulationVolumeR;
layout(binding = 4, rgba32f) uniform image3D lpvAccumulationVolumeG;
layout(binding = 5, rgba32f) uniform image3D lpvAccumulationVolumeB;

/*
 * Thanks to: https://github.com/mafian89/Light-Propagation-Volumes
*/

struct Contribution
{
	vec4 R;
	vec4 G;
	vec4 B;
};

#define PI 3.1415926f

/*Spherical harmonics coefficients - precomputed*/
#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2

/*Cosine lobe coeff*/
#define SH_cosLobe_C0 0.886226925f // sqrt(pi)/2 
#define SH_cosLobe_C1 1.02332671f // sqrt(pi/3) 

ivec2 cellSides[4] = { ivec2( 1.0, 0.0 ), ivec2( 0.0, 1.0 ), ivec2( -1.0, 0.0 ), ivec2( 0.0, -1.0 ) };

vec4 evalSH_direct( vec3 direction ) {	
	return vec4( SH_C0, -SH_C1 * direction.y, SH_C1 * direction.z, -SH_C1 * direction.x );
}

vec4 evalCosineLobeToDir_direct( vec3 direction ) {
	return vec4( SH_cosLobe_C0, -SH_cosLobe_C1 * direction.y, SH_cosLobe_C1 * direction.z, -SH_cosLobe_C1 * direction.x );
}

//Get side direction
vec3 getEvalSideDirection( int index, ivec3 orientation ) {
	const float smallComponent = 0.4472135; // 1 / sqrt(5)
	const float bigComponent = 0.894427; // 2 / sqrt(5)
	
	const ivec2 side = cellSides[ index ];
	vec3 tmp = vec3( side.x * smallComponent, side.y * smallComponent, bigComponent );
	return vec3(orientation.x * tmp.x, orientation.y * tmp.y, orientation.z * tmp.z);
}

vec3 getReprojSideDirection( int index, ivec3 orientation ) {
	const ivec2 side = cellSides[ index ];
	return vec3( orientation.x*side.x, orientation.y*side.y, 0 );
}

Contribution CalcPropagation (ivec3 volumePos)
{
	Contribution result;
	result.R = vec4 (0); // texelFetch(volumeTextureR, volumePos, 0);
	result.G = vec4 (0); // texelFetch(volumeTextureG, volumePos, 0);
	result.B = vec4 (0); // texelFetch(volumeTextureB, volumePos, 0);

	const ivec3 directions [6] = {
		ivec3(0,0,1), ivec3(0,0,-1),
		ivec3(1,0,0), ivec3(-1,0,0),
		ivec3(0,1,0), ivec3(0,-1,0)
	};

	const float directFaceSubtendedSolidAngle = 0.12753712; // 0.4006696846f / Pi;
	const float sideFaceSubtendedSolidAngle = 0.13478556; // 0.4234413544f / Pi;

	const vec3 volumeCellSize = 1.0f / volumeSize;

	for(int neighbour = 0; neighbour < 6; neighbour++) {

		//Get main direction
		ivec3 mainDirection = directions [neighbour]; 

		//get neighbour cell indexindex
		ivec3 neighbourCellIndex = volumePos - mainDirection;

		if (neighbourCellIndex.x >= volumeSize.x ||
			neighbourCellIndex.y >= volumeSize.y ||
			neighbourCellIndex.z >= volumeSize.z) {
			continue;
		}

		//Load sh coeffs
		vec4 RSHcoeffsNeighbour = texelFetch(volumeTextureR, neighbourCellIndex, 0);
		vec4 GSHcoeffsNeighbour = texelFetch(volumeTextureG, neighbourCellIndex, 0);
		vec4 BSHcoeffsNeighbour = texelFetch(volumeTextureB, neighbourCellIndex, 0);

		float occlusion = 1.0f;

		if (iteration > 0 && geometryOcclusion != 0) {
			vec3 occCoord = (vec3( neighbourCellIndex ) + 0.5 * mainDirection) * volumeCellSize;

			vec4 occCoefficients = texture (geometryTexture, occCoord);
			occlusion = 1.0 - clamp (dot (occCoefficients, evalSH_direct( -mainDirection )), 0.0, 1.0);
		}

		float occlusionDirectFace = occlusion * directFaceSubtendedSolidAngle;

		vec4 mainDirectionCosineLobeSH = evalCosineLobeToDir_direct( mainDirection );
		vec4 mainDirectionSH = evalSH_direct( mainDirection );

		result.R += occlusionDirectFace * max(0.0, dot( RSHcoeffsNeighbour, mainDirectionSH )) * mainDirectionCosineLobeSH;
		result.G += occlusionDirectFace * max(0.0, dot( GSHcoeffsNeighbour, mainDirectionSH )) * mainDirectionCosineLobeSH;
		result.B += occlusionDirectFace * max(0.0, dot( BSHcoeffsNeighbour, mainDirectionSH )) * mainDirectionCosineLobeSH;

		// Now we have contribution for the neighbour's cell in the main direction -> need to do reprojection 
		// Reprojection will be made only onto 4 faces (acctually we need to take into account 5 faces but we already have the one in the main direction)

		for(int face = 0; face < 4; face++) {
			//Get the direction to the face
			vec3 evalDirection = getEvalSideDirection( face, mainDirection );
			//Reprojected direction
			vec3 reprojDirection = getReprojSideDirection( face, mainDirection );

			float occlusion = 1.0f;

			if (iteration > 0 && geometryOcclusion != 0) {
				vec3 occCoord = (vec3( neighbourCellIndex ) + 0.5 * evalDirection) * volumeCellSize;

				vec4 occCoefficients = texture (geometryTexture, occCoord);
				occlusion = 1.0 - clamp (dot (occCoefficients, evalSH_direct( -evalDirection )), 0.0, 1.0);
			}

			float occlusionSideFace = occlusion * sideFaceSubtendedSolidAngle;

			//Get sh coeff
			vec4 reprojDirectionCosineLobeSH = evalCosineLobeToDir_direct( reprojDirection );
			vec4 evalDirectionSH = evalSH_direct( evalDirection );

			result.R += occlusionSideFace * max(0.0, dot( RSHcoeffsNeighbour, evalDirectionSH )) * reprojDirectionCosineLobeSH;
			result.G += occlusionSideFace * max(0.0, dot( GSHcoeffsNeighbour, evalDirectionSH )) * reprojDirectionCosineLobeSH;
			result.B += occlusionSideFace * max(0.0, dot( BSHcoeffsNeighbour, evalDirectionSH )) * reprojDirectionCosineLobeSH;
		}
	}

	return result;
}

void main() 
{
	if (gl_GlobalInvocationID.x < volumeSize.x
		&& gl_GlobalInvocationID.y < volumeSize.y
		&& gl_GlobalInvocationID.z < volumeSize.z) {

		ivec3 volumePos = ivec3(gl_GlobalInvocationID);

		Contribution result = CalcPropagation (volumePos);

		imageStore(lpvVolumeR, volumePos, result.R);
		imageStore(lpvVolumeG, volumePos, result.G);
		imageStore(lpvVolumeB, volumePos, result.B);

		if (iteration > 0) {
			result.R += imageLoad(lpvAccumulationVolumeR, volumePos);
			result.G += imageLoad(lpvAccumulationVolumeG, volumePos);
			result.B += imageLoad(lpvAccumulationVolumeB, volumePos);
		} else {
			result.R += texelFetch(volumeTextureR, volumePos, 0);
			result.G += texelFetch(volumeTextureG, volumePos, 0);
			result.B += texelFetch(volumeTextureB, volumePos, 0);
		}

		imageStore(lpvAccumulationVolumeR, volumePos, result.R);
		imageStore(lpvAccumulationVolumeG, volumePos, result.G);
		imageStore(lpvAccumulationVolumeB, volumePos, result.B);
	}
}