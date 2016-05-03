#version 330

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
uniform vec4 MaterialAmbient;
uniform vec4 MaterialSpecular;
uniform float MaterialShininess;
uniform float MaterialTransparency;

uniform sampler2D AmbientMap;
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D AlphaMap;

uniform vec3 cameraPosition;

uniform vec3 sceneAmbient;

in vec3 position;
in vec3 normal; 
in vec2 texcoord;

void main()
{
	float alphaMap = MaterialTransparency * texture2D (AlphaMap, texcoord.xy).x;

	if (alphaMap == 0) {
		discard;
	}

	vec3 ambientMap = vec3 (MaterialAmbient * texture2D (AmbientMap, texcoord.xy));
	vec3 diffuseMap = vec3 (MaterialDiffuse * texture2D (DiffuseMap, 	texcoord.xy));
	vec3 specularMap = vec3 (MaterialSpecular * texture2D (SpecularMap, texcoord.xy));

	// ambient lightDirection
	// vec3 totalLighting = vec3 (MaterialAmbient) * ambientMap * diffuseMap;

	// Renormalise normal vector
	vec3 norm = normalize (normal);

	out_position = vec4 (position, 1.0);
	out_diffuse = vec4 (diffuseMap, alphaMap);
	out_normal = vec4 (norm, 1.0);
	out_specular = vec4 (specularMap, MaterialShininess);
}