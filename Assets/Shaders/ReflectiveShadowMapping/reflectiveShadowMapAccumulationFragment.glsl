#version 330 core

layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_flux;

uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;

uniform float MaterialTransparency;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D AlphaMap;

const vec3 nullInAlphaMap = vec3 (0.0);

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

in vec3 vert_position;
in vec3 vert_normal; 
in vec2 vert_texcoord;

void main()
{
	/*
	 * Get color of diffuse and alpha map
	*/

	vec3 diffuseMap = MaterialDiffuse * vec3 (texture (DiffuseMap, vert_texcoord.xy));
	vec3 alphaMap = vec3 (texture (AlphaMap, vert_texcoord.xy));

	/*
	 * Check alpha texture
	*/

	if (alphaMap == nullInAlphaMap) {
		discard;
	}

	/*
	 * Discard translucent objects
	*/

	if (MaterialTransparency > 0) {
		discard;
	}

	/*
	 * Renormalize normal vector
	*/ 

	vec3 norm = normalize (vert_normal);

	/*
	 *
	*/

	vec3 flux = lightColor * diffuseMap * lightIntensity;

	/*
	 * Output texel
	*/

	out_position = vec4 (vert_position, 1.0);
	out_normal = vec4 (vert_normal, 1);
	out_flux = vec4 (flux, 1);
}
