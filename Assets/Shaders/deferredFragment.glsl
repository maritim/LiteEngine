#version 330 core

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_diffuse;
layout (location = 3) out vec4 out_specular;
layout (location = 4) out vec4 out_emissive;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform vec3 MaterialEmissive;

uniform float MaterialShininess;
uniform float MaterialTransparency;
uniform float MaterialRefractiveIndex;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D EmissiveMap;
uniform sampler2D AlphaMap;

uniform vec3 cameraPosition;

uniform float gamma;

const vec3 nullInAlphaMap = vec3 (0.0);

in vec3 geom_position;
in vec3 geom_normal; 
in vec2 geom_texcoord;

void main()
{
	/*
	 * Apply gamma on diffuse map
	*/

	vec3 srgbDiffuseMap = pow (vec3 (texture (DiffuseMap, geom_texcoord.xy)), vec3 (gamma));

	/*
	 * Get color of all used texture maps
	*/

	vec3 diffuseMap = MaterialDiffuse * srgbDiffuseMap;
	vec3 specularMap = MaterialSpecular * vec3 (texture (SpecularMap, geom_texcoord.xy));
	vec3 emissiveMap = MaterialEmissive * vec3 (texture (EmissiveMap, geom_texcoord.xy));
	vec3 alphaMap = vec3 (texture (AlphaMap, geom_texcoord.xy));

	/*
	 * Check alpha texture
	*/

	if (alphaMap == nullInAlphaMap) {
		discard;
	}

	/*
	 * Renormalize normal vector
	*/ 

	vec3 norm = normalize (geom_normal);

	/*
	 * Output texel for geometry pass in deferred rendering
	*/

	out_position = vec4 (geom_position, 1.0);
	out_diffuse = vec4 (diffuseMap, MaterialTransparency);
	out_normal = vec4 (norm, MaterialRefractiveIndex);
	out_specular = vec4 (specularMap, MaterialShininess);
	out_emissive = vec4 (emissiveMap, 1.0);
}
