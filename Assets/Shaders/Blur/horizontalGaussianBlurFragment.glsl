#version 330 core

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform sampler2D blurMap;

uniform vec2 blurMapResolution;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / blurMapResolution;
}

vec3 CalcBlur (vec2 texCoord)
{
	const float weight [5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

	vec3 color = texture (blurMap, texCoord).xyz * weight [0];

	vec2 texelSize = vec2 (1.0f) / blurMapResolution;

	for (int i = 1; i < 5; ++i) {
		color += texture (blurMap, texCoord + vec2 (texelSize.x * i, 0.0f)).rgb * weight [i];
		color += texture (blurMap, texCoord - vec2 (texelSize.x * i, 0.0f)).rgb * weight [i];
	}

	return color;
}

void main()
{
	vec2 texCoord = CalcTexCoord();

	out_color = CalcBlur (texCoord);
}
