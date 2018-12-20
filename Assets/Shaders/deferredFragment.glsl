#version 330 core

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_diffuse;
layout (location = 3) out vec4 out_specular;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;

uniform float MaterialShininess;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D AlphaMap;

uniform vec3 cameraPosition;

uniform vec3 sceneAmbient;

const vec3 nullInAlphaMap = vec3 (0.0);

in vec3 geom_position;
in vec3 geom_normal; 
in vec2 geom_texcoord;

void main()
{
	/*
	 * Apply gamma on diffuse map
	*/

	const float gamma = 2.2;

	vec3 srgbDiffuseMap = pow (vec3 (texture2D (DiffuseMap, geom_texcoord.xy)), vec3 (gamma));

	/*
	 * Get color of all used texture maps
	*/

	vec3 diffuseMap = MaterialDiffuse * srgbDiffuseMap;
	vec3 specularMap = MaterialSpecular * vec3 (texture2D (SpecularMap, geom_texcoord.xy));
	vec3 alphaMap = vec3 (texture2D (AlphaMap, geom_texcoord.xy));

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
	out_diffuse = vec4 (diffuseMap, 1);
	out_normal = vec4 (norm, 1.0);
	out_specular = vec4 (specularMap, 1.0);
}