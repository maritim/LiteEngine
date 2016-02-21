#version 330 core

layout(location = 0) out vec4 out_color;

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

uniform sampler2D TextureAtlas;

uniform vec3 cameraPosition;

struct LightSource
{
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection;
};

uniform vec3 sceneAmbient;

const int numberOfLights = 3;

uniform LightSource spotLights [numberOfLights];
uniform LightSource pointLights [numberOfLights];
uniform LightSource directionalLights[numberOfLights];

uniform int spotLightCount;
uniform int pointLightCount;
uniform int directionalLightCount;

in vec3 position;
in vec3 normal; 
in vec2 texcoord;

void main()
{
	float alphaMap = MaterialTransparency * texture2D (AlphaMap, texcoord.xy).x;

	if (alphaMap == 0) {
		discard;
	}

	vec3 ambientMap = vec3 (texture2D (AmbientMap, texcoord.xy));
	vec3 diffuseMap = vec3 (texture2D (DiffuseMap, 	texcoord.xy));
	vec3 specularMap = vec3 (texture2D (SpecularMap, texcoord.xy));

	// ambient lightDirection
	vec3 totalLighting = vec3 (MaterialAmbient) * ambientMap * sceneAmbient;

	// Renormalise normal vector
	vec3 norm = normalize (normal);

	for (int index = 0; index < directionalLightCount; index++ ) {
		// The position is also a direction for Directional Lights
		vec3 lightDirection = normalize ( vec3 (directionalLights [index].position));

		// Diffuse contribution
		float dCont = max (dot (norm, lightDirection), 0.0);

		// Attenuation is 1.0 for Directional Lights
		vec3 diffuseLighting = vec3 (directionalLights [index].diffuse) *
			vec3 (MaterialDiffuse) * diffuseMap * dCont;

		vec3 surface2view = normalize (cameraPosition - position);
		vec3 reflection = reflect (-lightDirection, norm);

		// Specular contribution
		float sCont = pow (max (dot (surface2view, reflection), 0.0), MaterialShininess);

		vec3 specular = vec3 (directionalLights [index].specular) * specularMap *
			vec3 (MaterialSpecular) * sCont;

		totalLighting = totalLighting + diffuseLighting + specular;
	}

	out_color = vec4 (totalLighting, alphaMap);
}