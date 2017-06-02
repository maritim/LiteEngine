#version 330

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec3 sceneAmbient;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightSpecularColor;

uniform vec3 attenuationComp;

uniform vec2 screenSize;

const int CASCADED_SHADOW_MAP_LEVELS = 4;

uniform sampler2D shadowMaps[CASCADED_SHADOW_MAP_LEVELS];

uniform mat4 lightSpaceMatrices[CASCADED_SHADOW_MAP_LEVELS];

uniform float clipZLevels[CASCADED_SHADOW_MAP_LEVELS];

uniform sampler3D volumeTexture;

uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;
uniform int volumeMipmapLevel;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

/*
 * Shadow Calculation
 * Thanks to: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
*/

float ShadowCalculation (vec4 lightSpacePos, int cascadedLevel)
{
    // perform perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	if(projCoords.z > 1.0)
        return 0.0;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture (shadowMaps [cascadedLevel], projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
	float bias = 0.0002;

    // Check whether current frag pos is in shadow
    float shadow = 0.5;

	vec2 texelSize = 1.0 / textureSize(shadowMaps [cascadedLevel], 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMaps [cascadedLevel], projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	shadow /= 9.0;

    return shadow;
}

int GetShadowCascadeLevel (float depth)
{
	for (int index = 0 ; index < CASCADED_SHADOW_MAP_LEVELS ; index++) {
        if (depth <= clipZLevels [index]) {
			return index;
        }
    }
}

float GetInterpolatedComp (float comp, float minValue, float maxValue, float domain)
{
	return ((comp - minValue) / (maxValue - minValue)) * domain;
}

vec3 GetPositionInVolume (vec3 origin, ivec3 volumeSize)
{
	vec3 positionInVolume;

	positionInVolume.x = GetInterpolatedComp (origin.x, minVertex.x, maxVertex.x, volumeSize.x);
	positionInVolume.y = GetInterpolatedComp (origin.y, minVertex.y, maxVertex.y, volumeSize.y);
	positionInVolume.z = GetInterpolatedComp (origin.z, minVertex.z, maxVertex.z, volumeSize.z);

	return positionInVolume;
}

float VOXEL_SIZE = (1.0 / volumeSize.x);
float MIPMAP_HARDCAP = 4;

float OFFSET = 8 * VOXEL_SIZE;
float STEP = VOXEL_SIZE;

bool IsInsideVolume (const vec3 p) 
{
	return (p.x < 1 && p.x >= 0.0 &&
		p.y < 1 && p.y >= 0.0 &&
		p.z < 1.0  && p.z >= 0.0);
}

// Traces a specular voxel cone.
vec3 traceSpecularVoxelCone (vec3 from, vec3 direction, vec3 in_normal) 
{
	direction = normalize(direction);

	from += OFFSET * in_normal;
	
	float dist = OFFSET;

	float MAX_DISTANCE = 100.0;

	// Trace.
	while (dist < MAX_DISTANCE){ 
		vec3 lobePosition = from + dist * direction;

		vec3 posInVolume = GetPositionInVolume (lobePosition, volumeSize);

		if (!IsInsideVolume (posInVolume)) {
			break;
		}
		
		float level = log2(1 + dist / VOXEL_SIZE);

		vec4 voxelColor = vec4 (1.0);// textureLod (volumeTexture, posInVolume, min(level, MIPMAP_HARDCAP));

		if (voxelColor.a > 0) {
			return voxelColor.rgb * 0.5;
		}

		dist += STEP * (1.0f + 0.125f * level);
	}

	return vec3 (0.0);
}

// Calculates indirect specular light using voxel cone tracing.
vec3 CalcIndirectSpecularLight (vec3 in_position, vec3 in_normal) 
{
	vec3 viewDirection = normalize (in_position - cameraPosition);

	vec3 reflection = normalize(reflect(viewDirection, in_normal));
	
	return traceSpecularVoxelCone (in_position, reflection, in_normal);
}

vec3 CalcDirectLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	// Diffuse contribution
	float dCont = max (dot (in_normal, lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuseColor = lightColor * in_diffuse * dCont;

	vec3 surface2view = normalize (cameraPosition - in_position);
	vec3 reflection = reflect (-lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), 1.0);

	vec3 specularColor = lightSpecularColor * in_diffuse * sCont;

	// Calculate shadow level
	vec4 clipPos = (viewProjectionMatrix * vec4 (in_position, 1.0));
	float depth = clipPos.z / clipPos.w;
	int shadowCascadedLevel = GetShadowCascadeLevel (depth);

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrices [shadowCascadedLevel] * vec4 (in_position, 1.0f);
	float shadow = ShadowCalculation (lightSpacePos, shadowCascadedLevel);

	vec3 multi = vec3 (0);

	// if (shadowCascadedLevel == 0)
	// 	multi.r = 1;
	// else if (shadowCascadedLevel == 1)
	// 	multi.g = 1;
	// else
	// 	multi.b = 1;

	return multi + (1.0 - shadow) * (diffuseColor + specularColor);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_specular = texture2D (gSpecularMap, texCoord).xyz;
	float in_shininess = texture2D (gSpecularMap, texCoord).w;

	in_normal = normalize(in_normal);

	out_color = CalcDirectLight(in_position, in_normal, in_diffuse, in_specular, in_shininess);
	out_color += CalcIndirectSpecularLight (in_position, in_normal);

	//out_color = texture2D (shadowMap, texCoord).xyz;// + vec3 (0.2, 0, 0);
} 