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

float GetInterpolatedComp (float comp, float minValue, float maxValue)
{
	return ((comp - minValue) / (maxValue - minValue));
}

vec3 GetPositionInVolume (vec3 origin)
{
	vec3 positionInVolume;

	positionInVolume.x = GetInterpolatedComp (origin.x, minVertex.x, maxVertex.x);
	positionInVolume.y = GetInterpolatedComp (origin.y, minVertex.y, maxVertex.y);
	positionInVolume.z = GetInterpolatedComp (origin.z, minVertex.z, maxVertex.z);

	return positionInVolume + vec3 (1.0 / volumeSize.x);
}

bool IsInsideVolume (const vec3 p) 
{
	return (p.x < 1 && p.x >= 0.0 &&
		p.y < 1 && p.y >= 0.0 &&
		p.z < 1.0  && p.z >= 0.0);
}

ivec2 GetInterpolationComp (float comp, float domain)
{
	return ivec2 (floor (comp * domain), ceil (comp * domain));
}

float VOXEL_SIZE = ((maxVertex.x - minVertex.x) / volumeSize.x);
float MIPMAP_HARDCAP = 5;

float minVoxelDiameter = 1.0 / volumeSize.x;
float minVoxelDiameterInv = volumeSize.x;

// Returns a vector that is orthogonal to u.
vec3 orthogonal(vec3 u){
	u = normalize(u);
	vec3 v = vec3(1.0, 0.0, 0.0); // Pick any normalized vector.
	return abs(dot(u, v)) > 0.99999f ? cross(u, vec3(0, 1, 0)) : cross(u, v);
}

// origin, dir, and maxDist are in texture space
// dir should be normalized
// coneRatio is the cone diameter to height ratio (2.0 for 90-degree cone)
vec3 voxelTraceCone(vec3 origin, vec3 dir, float coneRatio, float maxDist)
{
	vec3 samplePos = origin;
	vec4 accum = vec4(0.0);

	// the starting sample diameter
	float minDiameter = minVoxelDiameter;

	// push out the starting point to avoid self-intersection
	float startDist = minDiameter * 20;
	
	float dist = startDist;
	while (dist <= maxDist && accum.a < 1.0)
	{
		// ensure the sample diameter is no smaller than the min
		// desired diameter for this cone (ensuring we always
		// step at least minDiameter each iteration, even for tiny
		// cones - otherwise lots of overlapped samples)
		float sampleDiameter = max(minDiameter, coneRatio * dist);
		
		// convert diameter to LOD
		// for example:
		// log2(1/256 * 256) = 0
		// log2(1/128 * 256) = 1
		// log2(1/64 * 256) = 2
		float sampleLOD = log2(sampleDiameter * minVoxelDiameterInv);
		
		vec3 samplePos = origin + dir * dist;
		
		vec4 sampleValue = textureLod (volumeTexture, samplePos, min (sampleLOD, MIPMAP_HARDCAP));
		
		accum.rgb += sampleValue.rgb * sampleValue.a;
		accum.a += sampleValue.a;

		dist += sampleDiameter;
	}
	
	// decompress color range to decode limited HDR
	accum.xyz *= 2.0;
	
	return accum.xyz;
}

// Calculates indirect diffuse light using voxel cone tracing.
vec3 CalcIndirectDiffuseLight(vec3 in_position, vec3 in_normal)
{
	vec3 voxelPos = GetPositionInVolume (in_position);

	vec3 tangent = normalize(orthogonal(in_normal));
	vec3 bitangent = normalize(cross(tangent, in_normal));

	vec3 iblDiffuse = vec3(0.0);

	float iblConeRatio = 2;
	float iblMaxDist = .3;
	// this sample gets full weight (dot(normal, normal) == 1)
	iblDiffuse += voxelTraceCone(voxelPos, in_normal, iblConeRatio, iblMaxDist).xyz;
	// these samples get partial weight
	iblDiffuse += .707 * voxelTraceCone(voxelPos, normalize(in_normal + tangent), iblConeRatio, iblMaxDist).xyz;
	iblDiffuse += .707 * voxelTraceCone(voxelPos, normalize(in_normal - tangent), iblConeRatio, iblMaxDist).xyz;
	iblDiffuse += .707 * voxelTraceCone(voxelPos, normalize(in_normal + bitangent), iblConeRatio, iblMaxDist).xyz;
	iblDiffuse += .707 * voxelTraceCone(voxelPos, normalize(in_normal - bitangent), iblConeRatio, iblMaxDist).xyz;

	// Return result.
	return iblDiffuse;
}

// Calculates indirect specular light using voxel cone tracing.
vec3 CalcIndirectSpecularLight (vec3 in_position, vec3 in_normal) 
{
	vec3 specularLight;

	vec3 eyeToFragment = normalize (in_position - cameraPosition);
	vec3 reflectionDir = reflect (eyeToFragment, in_normal);
		
	vec3 reflectTraceOrigin = GetPositionInVolume (in_position);
	float coneRatio = .2;
	float maxDist = .3;
	specularLight = voxelTraceCone (reflectTraceOrigin, reflectionDir, coneRatio, maxDist).xyz;

	return specularLight;
}

vec3 CalcDirectDiffuseLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	// Diffuse contribution
	float dCont = max (dot (in_normal, lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuseColor = lightColor * dCont;

	return diffuseColor;
}

vec3 CalcDirectSpecularLight (vec3 in_position, vec3 in_normal, vec3 in_specular, float in_shininess)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	vec3 surface2view = normalize (cameraPosition - in_position);
	vec3 reflection = reflect (-lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), 10.0);

	vec3 indirectSpecularColor = CalcIndirectSpecularLight (in_position, in_normal);

	vec3 specularColor = (lightSpecularColor + indirectSpecularColor) * sCont;

	return indirectSpecularColor;
}

float CalcShadowContribution (vec3 in_position)
{
	// Calculate shadow level
	vec4 clipPos = (viewProjectionMatrix * vec4 (in_position, 1.0));
	float depth = clipPos.z / clipPos.w;
	int shadowCascadedLevel = GetShadowCascadeLevel (depth);

	// Calculate shadow
	vec4 lightSpacePos = lightSpaceMatrices [shadowCascadedLevel] * vec4 (in_position, 1.0f);
	float shadow = ShadowCalculation (lightSpacePos, shadowCascadedLevel);

	return shadow;
}

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	vec3 directDiffuseColor = CalcDirectDiffuseLight (in_position, in_normal, in_diffuse);
	vec3 directSpecularColor = CalcDirectSpecularLight (in_position, in_normal, in_specular, in_shininess);

	float shadow = CalcShadowContribution (in_position);

	directDiffuseColor = (1.0 - shadow) * (directDiffuseColor);

	// Calculate indirect diffuse lighting
	vec3 indirectDiffuseColor = CalcIndirectDiffuseLight (in_position, in_normal);

	return (directDiffuseColor + indirectDiffuseColor) * in_diffuse
		   + (directSpecularColor) * in_specular;
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

	out_color = CalcDirectionalLight (in_position, in_normal, in_diffuse, in_specular, in_shininess);
} 