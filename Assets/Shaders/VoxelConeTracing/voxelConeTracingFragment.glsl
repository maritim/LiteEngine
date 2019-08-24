#version 330

layout(location = 0) out vec3 out_color;

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

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 attenuationComp;

uniform vec2 screenSize;

uniform sampler3D volumeTexture;

uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;
uniform int volumeMipmapLevels;

uniform float diffuseConeDistance;
uniform float specularConeDistance;
uniform float shadowConeRatio;
uniform float shadowConeDistance;

uniform float indirectIntensity;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
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

float minVoxelDiameter = 1.0 / volumeSize.x;
float minVoxelDiameterInv = volumeSize.x;

/*
 * Calculate  a vector that is orthogonal to u.
*/

vec3 Orthogonal(vec3 u)
{
	u = normalize(u);

	vec3 v = vec3(0.0, 1.0, 0.0);

	return abs(dot(u, v)) > 0.99 ? cross(u, vec3(0, 0, 1)) : cross(u, v);
}

// origin, dir, and maxDist are in texture space
// dir should be normalized
// coneRatio is the cone diameter to height ratio (2.0 for 90-degree cone)
vec4 voxelTraceCone(vec3 origin, vec3 dir, float coneRatio, float maxDist)
{
	vec3 samplePos = origin;
	vec3 accum = vec3(0.0);
	float alpha = 0.0;

	// the starting sample diameter
	float minDiameter = minVoxelDiameter;

	// push out the starting point to avoid self-intersection
	float startDist = minDiameter * 3;
	
	float dist = startDist;
	while (dist <= maxDist && alpha < 1.0)
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
		
		vec4 sampleValue = textureLod (volumeTexture, samplePos, min (sampleLOD, volumeMipmapLevels - 1.0));
		
		accum = accum + (1.0 - alpha) * sampleValue.a * sampleValue.rgb;
		alpha = alpha + (1.0 - alpha) * sampleValue.a;

		dist += sampleDiameter;
	}
	
	// decompress color range to decode limited HDR
	// accum *= 2.0;
	
	return vec4 (accum, alpha);
}

// Calculates indirect diffuse light using voxel cone tracing.
vec3 CalcIndirectDiffuseLight(vec3 in_position, vec3 in_normal)
{
	vec3 voxelPos = GetPositionInVolume (in_position);

	vec3 tangent = normalize(Orthogonal(in_normal));
	vec3 bitangent = normalize(cross(tangent, in_normal));

	vec3 iblDiffuse = vec3(0.0);

	float iblConeRatio = 1;

	// this sample gets full weight (dot(normal, normal) == 1)
	iblDiffuse += voxelTraceCone(voxelPos, in_normal, iblConeRatio, diffuseConeDistance).xyz;

	// these samples get partial weight
	iblDiffuse += voxelTraceCone(voxelPos, normalize(in_normal + tangent), iblConeRatio, diffuseConeDistance).xyz;
	iblDiffuse += voxelTraceCone(voxelPos, normalize(in_normal - tangent), iblConeRatio, diffuseConeDistance).xyz;
	iblDiffuse += voxelTraceCone(voxelPos, normalize(in_normal + bitangent), iblConeRatio, diffuseConeDistance).xyz;
	iblDiffuse += voxelTraceCone(voxelPos, normalize(in_normal - bitangent), iblConeRatio, diffuseConeDistance).xyz;

	// Return result.
	return iblDiffuse;
}

// Calculates indirect specular light using voxel cone tracing.
vec3 CalcIndirectSpecularLight (vec3 in_position, vec3 in_normal, float in_shininess)
{
	vec3 specularLight;

	vec3 eyeToFragment = normalize (in_position - cameraPosition);
	vec3 reflectionDir = reflect (eyeToFragment, in_normal);

	float specularConeRatio = pow (0.99, in_shininess);
		
	vec3 reflectTraceOrigin = GetPositionInVolume (in_position);
	specularLight = voxelTraceCone (reflectTraceOrigin, reflectionDir, specularConeRatio, specularConeDistance).xyz;

	return specularLight;
}

vec3 CalcDirectDiffuseLight (vec3 in_position, vec3 in_normal)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	// Diffuse contribution
	float dCont = max (dot (in_normal, lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuseColor = lightColor * dCont;

	return diffuseColor;
}

vec3 CalcDirectSpecularLight (vec3 in_position, vec3 in_normal, float in_shininess)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	vec3 surface2view = normalize (cameraPosition - in_position);
	vec3 reflection = reflect (-lightDirection, in_normal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), in_shininess);

	vec3 specularColor = lightColor * sCont;

	return specularColor;
}

float voxelTraceConeOcclusion(vec3 origin, vec3 dir, float coneRatio, float maxDist)
{
	vec3 samplePos = origin;
	float occlusion = 0.0;
	float alpha = 0.0;

	// the starting sample diameter
	float minDiameter = minVoxelDiameter;

	// push out the starting point to avoid self-intersection
	float startDist = minDiameter;
	
	float dist = startDist;
	while (dist <= maxDist && alpha < 1.0)
	{
		float sampleDiameter = max(minDiameter, coneRatio * dist);
		
		float sampleLOD = log2(sampleDiameter * minVoxelDiameterInv);
		
		vec3 samplePos = origin + dir * dist;
		
		vec4 sampleValue = textureLod (volumeTexture, samplePos, min (sampleLOD, volumeMipmapLevels - 1.0));

		occlusion += ((1.0 - alpha) * sampleValue.a) / (1.0 + 0.03 * sampleDiameter);

		alpha = alpha + (1.0 - alpha) * sampleValue.a;

		dist += sampleDiameter;
	}
	
	return occlusion;
}

float CalcOcclusion (vec3 in_position, vec3 in_normal)
{
	vec3 voxelPos = GetPositionInVolume (in_position);

	vec3 tangent = normalize(Orthogonal(in_normal));
	vec3 bitangent = normalize(cross(in_normal, tangent));

	float occlusion = 0.0;

	float iblConeRatio = 1;
	float iblMaxDist = .1;

	// this sample gets full weight (dot(normal, normal) == 1)
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, in_normal, iblConeRatio, iblMaxDist);

	// these samples get partial weight
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(in_normal + tangent), iblConeRatio, iblMaxDist);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(in_normal - tangent), iblConeRatio, iblMaxDist);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(in_normal + bitangent), iblConeRatio, iblMaxDist);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(in_normal - bitangent), iblConeRatio, iblMaxDist);

	// Return result.
	return occlusion / 5.0;
}

float CalcShadow (vec3 in_position)
{
	vec3 voxelPos = GetPositionInVolume (in_position);

	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (lightPosition);

	float occlusion = voxelTraceConeOcclusion (voxelPos, lightDirection, shadowConeRatio, shadowConeDistance);

	return 1.0 - occlusion;
}

vec3 CalcDirectionalLight (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec3 in_specular, float in_shininess)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 directDiffuseColor = CalcDirectDiffuseLight (worldPosition, worldNormal);
	vec3 directSpecularColor = CalcDirectSpecularLight (worldPosition, worldNormal, in_shininess);

	float shadow = CalcShadow (worldPosition);

	directDiffuseColor = shadow * directDiffuseColor;
	directSpecularColor = shadow * directSpecularColor;

	vec3 indirectDiffuseColor = CalcIndirectDiffuseLight (worldPosition, worldNormal);
	vec3 indirectSpecularColor = CalcIndirectSpecularLight (worldPosition, worldNormal, in_shininess);

	float ambientOcclusion = CalcOcclusion (worldPosition, worldNormal);

	// return vec3 (ambientOcclusion);
	// return indirectSpecularColor;
	// return vec3 (shadow);
	// return indirectDiffuseColor;
	// return indirectSpecularColor;
	return (directDiffuseColor + indirectDiffuseColor * indirectIntensity) * in_diffuse
		   + (directSpecularColor + indirectSpecularColor * indirectIntensity) * in_specular;
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