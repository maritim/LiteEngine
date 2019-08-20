#version 420

layout (location = 0) out vec3 fragColor;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;
uniform mat3 inverseNormalWorldMatrix;

uniform vec2 screenSize;

uniform mat4 rsmLightSpaceMatrix;

uniform vec2 rsmResolution;

layout(binding = 0, RGBA32F) writeonly uniform image2D rsmCacheTexture;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

void CalcCaching (vec3 in_position)
{
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec4 lightSpacePos = rsmLightSpaceMatrix * vec4 (worldPosition, 1.0);
	vec3 rsmProjCoords = lightSpacePos.xyz / lightSpacePos.w;

	rsmProjCoords = rsmProjCoords * 0.5 + 0.5;

	ivec2 rsmPos = ivec2 (rsmProjCoords.xy * rsmResolution);

	imageStore(rsmCacheTexture, rsmPos, vec4 (0, 0, 0, 1));
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;

	CalcCaching(in_position);
} 