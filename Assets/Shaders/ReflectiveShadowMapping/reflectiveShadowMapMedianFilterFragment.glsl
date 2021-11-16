#version 330 core

layout(location = 0) out vec3 out_color;

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

uniform sampler2D indirectMap;

uniform vec2 rsmResolution;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / rsmResolution;
}

float Grayscale (vec3 color)
{
	return color.r * 0.3 + color.g * 0.59 + color.b * 0.11;
}

vec3 CalcMedian (vec2 texCoord)
{
	vec3 v[9];

	for (int x = -1; x <= 1; x += 1) {
		for (int y = -1; y <= 1; y += 1) {
			v[(x + 1) * 3 + y + 1] = texture(indirectMap, texCoord + vec2(float (x), float (y)) / rsmResolution).xyz;
		}
	}

	for (int i = 0; i < 9; i++) {
		for (int j = i+1; j < 9; j++) {
			if (Grayscale (v [i]) > Grayscale (v [j])) {
				vec3 aux = v [i];
				v [i] = v [j];
				v [j] = aux;
			}
		}
	}

	return v [4];
}

void main()
{
	vec2 texCoord = CalcTexCoord();

	out_color = CalcMedian (texCoord);
}
