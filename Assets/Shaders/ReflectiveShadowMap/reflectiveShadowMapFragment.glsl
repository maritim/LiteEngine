#version 330 core

layout(location = 0) out vec4 out_depth;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;
layout(location = 3) out vec4 out_flux;

uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D AlphaMap;

const vec3 nullInAlphaMap = vec3 (0.0);

in vec3 vert_position;
in vec3 vert_normal; 
in vec2 vert_texcoord;

void main()
{
	/*
	 * Get color of diffuse and alpha map
	*/

	vec3 diffuseMap = MaterialDiffuse * vec3 (texture2D (DiffuseMap, vert_texcoord.xy));
	vec3 alphaMap = vec3 (texture2D (AlphaMap, vert_texcoord.xy));

	/*
	 * Check alpha texture
	*/

	if (alphaMap == nullInAlphaMap) {
		discard;
	}

	/*
	 * Renormalize normal vector
	*/ 

	vec3 norm = normalize (vert_normal);

	/*
	 * Output texel
	*/

	out_position = vec4 (vert_position, 1.0);
	out_normal = vec4 (vert_normal, 1);
	out_flux = vec4 (diffuseMap, 1);
	out_depth = vec4 (gl_FragCoord.z);
}
