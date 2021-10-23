#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define VERTEX_COUNT 3

in vec3 vert_position[];
in vec3 vert_normal[];
in vec2 vert_texcoord[];

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

uniform int vplsCount;
uniform int emissiveTextured;

out vec3 geom_worldPosition;
out vec3 geom_worldNormal;
out vec2 geom_texcoord;

struct VPLSample
{
	vec4 position;
	vec4 normal;
	vec4 flux;
};

layout(std430, binding = 0) buffer VPLSamples
{
	VPLSample vplSamples[];
};

// float rand(vec2 co){
//   return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
// }

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

	VPLSample vplSample;

	vplSample.position.xyz =
		(1 - r1) * vert_position [0] +
		r1 * (1 - r2) * vert_position [1] +
		r1 * r2 * vert_position [2];

	vplSample.normal.xyz =
		(1 - r1) * vert_normal [0] +
		r1 * (1 - r2) * vert_normal [1] +
		r1 * r2 * vert_normal [2];

	vec3 emissiveMap = MaterialEmissive;

	if (emissiveTextured == 1) {
		vec2 texcoord =
			(1 - r1) * vert_texcoord [0] +
			r1 * (1 - r2) * vert_texcoord [1] +
			r1 * r2 * vert_texcoord [2];

		emissiveMap = emissiveMap * vec3 (texture2D (EmissiveMap, texcoord.xy));
	}

	vplSample.flux.rgb = emissiveMap;

	vplSamples [gl_PrimitiveIDIn * vplsCount + iteration] = vplSample;
}

void main ()
{
	for (int i = 0; i < vplsCount; i++) {
		GenerateVPL (i);
	}

	EndPrimitive ();

}
