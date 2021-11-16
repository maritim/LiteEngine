uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;

uniform int ambientOcclusionEnabled;
uniform sampler2D ambientOcclusionMap;

vec3 CalcAmbientLight ()
{
	float ambientOcclusion = 1.0f;

	if (ambientOcclusionEnabled == 1) {
		vec2 texCoord = CalcTexCoord();
		float in_ao = texture (ambientOcclusionMap, texCoord).x;

		ambientOcclusion = in_ao;
	}

	return ambientLightColor * ambientLightIntensity * ambientOcclusion;
}
