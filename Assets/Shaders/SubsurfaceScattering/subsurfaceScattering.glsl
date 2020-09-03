uniform sampler2D subsurfaceScatteringMap;

vec3 CalcSubsurfaceScatteringLight ()
{
	vec2 texCoord = CalcTexCoord();

	vec3 in_subsurfaceScattering = texture2D (subsurfaceScatteringMap, texCoord).xyz;

	return in_subsurfaceScattering;
}
