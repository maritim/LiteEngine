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

uniform vec2 bloomResolution;
uniform float bloomThreshold;

uniform sampler2D postProcessMap;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / bloomResolution;
}

vec3 CalcBright (vec3 in_diffuse)
{
	float brightness = dot (in_diffuse, vec3 (0.2126, 0.7152, 0.0722));

	if (brightness > bloomThreshold) {
		return in_diffuse;
	}

	return vec3 (0.0f);
}

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;

	out_color = CalcBright (in_diffuse);
}
