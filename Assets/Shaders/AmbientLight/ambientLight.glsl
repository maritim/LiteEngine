uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;

uniform sampler2D ambientOcclusionMap;

vec3 CalcAmbientLight (vec3 in_diffuse)
{
	vec2 texCoord = CalcTexCoord();
	float in_ao = texture2D (ambientOcclusionMap, texCoord).x;

	return in_diffuse * ambientLightColor * ambientLightIntensity * in_ao;
}