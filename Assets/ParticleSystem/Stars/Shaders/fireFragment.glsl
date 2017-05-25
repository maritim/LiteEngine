#version 330

layout(location = 0) out vec4 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 MaterialDiffuse;
uniform vec3 MaterialAmbient;
uniform vec3 MaterialSpecular;
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

uniform float texBlending;

in vec3 position;
in vec3 normal; 
in vec2 texcoordCurr;
in vec2 texcoordNext;

void main()
{
	float alphaMap = MaterialTransparency * texture2D (AlphaMap, texcoordCurr).x;

	if (alphaMap == 0) {
		discard;
	}

	vec4 mapColorCurr = texture2D (TextureAtlas, texcoordCurr.xy);
	vec4 mapColorNext = texture2D (TextureAtlas, texcoordNext.xy);

	// ambient lightDirection
	vec4 totalLighting = mix (mapColorCurr, mapColorNext, texBlending);

	// It's black
	// if (length (totalLighting) <= 0.15) {
	// 	discard;
	// }

	out_color = totalLighting;
}