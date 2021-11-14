#version 330

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
in vec4 texcoord;

void main()
{
	float alphaMap = MaterialTransparency * texture (AlphaMap, texcoord.xy).x;

	if (alphaMap == 0) {
		discard;
	}

	vec3 ambientMap = vec3 (texture (AmbientMap, texcoord.xy));
	vec3 diffuseMap = vec3 (texture (DiffuseMap, 	texcoord.xy));
	vec3 specularMap = vec3 (texture (SpecularMap, texcoord.xy));

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

	for (int index = 0; index < pointLightCount; index++ ) {
		// Vector from Light Source to Fragment
		vec3 lightDirection = vec3 (pointLights [index].position) - position;

		// Distance from Light Source to Fragment
		float dist = length (lightDirection);
			lightDirection = normalize (lightDirection);
		float attenuation = 1.0 / (pointLights[index].constantAttenuation
			       + pointLights[index].linearAttenuation * dist
			       + pointLights[index].quadraticAttenuation * dist * dist);

		// Diffuse contribution
		float dCont = max (dot (norm, lightDirection), 0.0);

		vec3 diffuseLighting = vec3 (pointLights [index].diffuse) *
			vec3 (MaterialDiffuse) * diffuseMap * dCont * attenuation;

		// Vector from Camera Positon to Fragment
		vec3 surface2view = normalize (cameraPosition - position);
		vec3 reflection = reflect (-lightDirection, norm);

		// Specular contribution
		float sCont = pow (max (dot (surface2view, reflection), 0.0), MaterialShininess);

		vec3 specular = vec3 (pointLights [index].specular) * specularMap *
			vec3 (MaterialSpecular) * sCont * attenuation;

		totalLighting = totalLighting + diffuseLighting + specular;
	}

	for (int index = 0; index < spotLightCount; index++ ) {
		// Vector from Light Source to Fragment
		vec3 lightDirection = vec3 (spotLights [index].position) - position;

		// Distance from Light Source to Fragment
		float dist = length (lightDirection);
		lightDirection = normalize (lightDirection);
		float attenuation = 1.0 / (pointLights[index].constantAttenuation
			       + pointLights[index].linearAttenuation * dist
			       + pointLights[index].quadraticAttenuation * dist * dist);

		// If it's a Spot Light we calculate the angle between Fragment to Light Source
		// position vector and Spot Cone to get the correct intensity of the light

		// WE NEED TO CHANGE THIS WITH INTERPOLATION TO MINIMIZE GPU USAGE
	    float clampedCosine = max(0.0, dot(-lightDirection, normalize(spotLights[index].spotDirection)));
	    if (clampedCosine < cos(radians(spotLights[index].spotCutoff))) // outside of spotlight cone?
		{
			attenuation = 0.0;
		}
	    else
		{
			attenuation = attenuation * pow(clampedCosine, spotLights[index].spotExponent);   
		}

		// Diffuse contribution
		float dCont = max (dot (norm, lightDirection), 0.0);

		vec3 diffuseLighting = vec3 (spotLights [index].diffuse) *
			vec3 (MaterialDiffuse) * diffuseMap * dCont * attenuation;

		// Vector from Camera Positon to Fragment
		vec3 surface2view = normalize (cameraPosition - position);
		vec3 reflection = reflect (-lightDirection, norm);

		// Specular contribution
		float sCont = pow (max (dot (surface2view, reflection), 0.0), MaterialShininess);

		vec3 specular = vec3 (spotLights [index].specular) * specularMap *
			vec3 (MaterialSpecular) * sCont * attenuation;

		totalLighting = totalLighting + diffuseLighting + specular;
	}	

	// // vector which point from pixel to light
	// vec3 surface2light = normalize(lightPos - position );

	// // diffuse contribution
	// float dCont = max (dot (norm, surface2light), 0.0);

	// vec3 diffuse = vec3 (gl_FrontMaterial.diffuse * gl_LightSource [0].diffuse * dCont);

	// float dist = length (position - lightPos);
	// float att = 1.0 / (1.0 + 1.0 * dist + 0.01 * dist * dist);

	// vec3 surface2view = normalize (-position);
	// // the reflection from light vector from normal
	// vec3 reflection = -reflect (surface2light, norm);

	// float sCont = max (dot (surface2view, reflection), 0.0);
	// sCont = pow (sCont, gl_FrontMaterial.shininess);

	// vec3 specular = vec3 (gl_FrontMaterial.specular * gl_LightSource [0].specular * sCont);

	out_color = vec4 (totalLighting, alphaMap);
}
