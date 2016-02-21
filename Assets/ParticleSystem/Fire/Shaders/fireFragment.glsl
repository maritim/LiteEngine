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

uniform sampler2D TextureAtlas;

uniform vec3 cameraPosition;

in vec3 position;
in vec3 normal; 
in vec2 texcoordCurr;
in vec2 texcoordNext;
in float blending;

void main()
{
	vec4 mapColorCurr = texture2D (TextureAtlas, texcoordCurr.xy);
	vec4 mapColorNext = texture2D (TextureAtlas, texcoordNext.xy);

	// ambient lightDirection
	vec4 totalLighting = mix (mapColorCurr, mapColorNext, blending);

	// It's black
	if (length (totalLighting) <= 0.15) {
		discard;
	}

	out_color = totalLighting;
	out_color = vec4 (1, 0, 0, 1);
}