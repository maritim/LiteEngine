#version 330 core

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;
uniform sampler2D gDepthMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;

uniform vec3 cameraPosition;

uniform vec2 screenSize;

uniform sampler2D postProcessMap;

uniform sampler2D reflectionMap;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture (gNormalMap, texCoord).xyz;
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;
	vec2 in_reflection = texture (reflectionMap, texCoord).xy;

	in_normal = normalize (in_normal);

	vec3 reflection = texture (postProcessMap, in_reflection).xyz;

	if (length (in_reflection) == 0.0f) {
		reflection = vec3 (0.0f);
	}

	float screenEdgeFade = 1.0f - clamp (length (vec2 (0.5f) - in_reflection), 0.0f, 0.5f) * 2.0f;

	vec3 reflectionDirection = normalize (reflect (normalize (in_position), in_normal));

    vec3 fresnel = fresnelSchlick(max(dot(in_normal, normalize(-in_position)), 0.0), vec3 (0.0f));

	out_color = in_diffuse + reflection * screenEdgeFade * clamp (-reflectionDirection.z, 0.0f, 1.0f) * fresnel;
}
