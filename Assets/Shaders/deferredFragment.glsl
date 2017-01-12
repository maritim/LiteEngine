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

uniform vec4 MaterialDiffuse;

uniform sampler2D DiffuseMap;

uniform vec3 cameraPosition;

uniform vec3 sceneAmbient;

in vec3 geom_position;
in vec3 geom_normal; 
in vec2 geom_texcoord;

void main()
{
	/*
	 * Get color of all used texture maps
	*/

	vec3 diffuseMap = vec3 (MaterialDiffuse * texture2D (DiffuseMap, 	geom_texcoord.xy));

	// ambient lightDirection
	// vec3 totalLighting = vec3 (MaterialAmbient) * ambientMap * diffuseMap;

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
	out_specular = vec4 (diffuseMap, 1);
}