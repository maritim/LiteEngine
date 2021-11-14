#version 330 core

layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_flux;

uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D AlphaMap;

const vec3 nullInAlphaMap = vec3 (0.0);

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRange;
uniform float lightSpotCutoff;
uniform float lightSpotOuterCutoff;

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
	 * Renormalize normal vector
	*/ 

	vec3 norm = normalize (vert_normal);

	/*
	 *
	*/

	vec3 lightDir = normalize (lightPosition - vert_position);
	float theta = dot (lightDir, -lightDirection);
	float epsilon = lightSpotCutoff - lightSpotOuterCutoff;
	float spotAttenuation = clamp ((theta - lightSpotOuterCutoff) / epsilon, 0.0, 1.0);

	vec3 flux = lightColor * diffuseMap * spotAttenuation * lightIntensity;

	/*
	 * Output texel
	*/

	out_position = vec4 (vert_position, 1.0);
	out_normal = vec4 (vert_normal, 1);
	out_flux = vec4 (flux, 1);
}
