#version 330 core

layout(location = 0) out float out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec2 screenSize;

uniform sampler2D ambientOcclusionMap;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

float CalcBlur (vec2 texCoord)
{
	float color = 0.0;

	vec2 texelSize = 1.0 / vec2 (textureSize (ambientOcclusionMap, 0));

	for (int x = -2; x < 2; ++x) {
		for (int y = -2; y < 2; ++y) {
			vec2 offset = vec2 (float(x), float(y)) * texelSize;
			color += texture2D (ambientOcclusionMap, texCoord + offset).r;
		}
	}

	color = color / (4.0 * 4.0);

	return color;
}

void main()
{
	vec2 texCoord = CalcTexCoord();

	out_color = CalcBlur (texCoord);
}
