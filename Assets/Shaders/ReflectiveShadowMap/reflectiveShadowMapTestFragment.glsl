#version 330 core

layout(location = 0) out vec3 out_color;

uniform sampler2D reflectiveShadowMap;
uniform sampler2D reflectiveShadowMapPosMap;
uniform sampler2D reflectiveShadowMapNormalMap;
uniform sampler2D reflectiveShadowMapFluxMap;

in vec2 geom_ScreenCoordinates;

int CalcCadran ()
{
	if (geom_ScreenCoordinates.x < 0 && geom_ScreenCoordinates.y < 0) {
		return 1;
	}

	if (geom_ScreenCoordinates.x < 0 && geom_ScreenCoordinates.y >= 0) {
		return 2;
	}

	if (geom_ScreenCoordinates.y < 0) {
		return 3;
	}

	if (geom_ScreenCoordinates.y >= 0) {
		return 4;
	}
}

vec3 CalcColor (int cadran)
{
	vec2 normalizedScreenPos = vec2 (
		(geom_ScreenCoordinates.x + 1.0) / 2.0, 
		(geom_ScreenCoordinates.y + 1.0) / 2.0
	);

	return texture2D (reflectiveShadowMap, normalizedScreenPos).xyz;

	if (cadran == 1) {
		return texture2D (reflectiveShadowMap, normalizedScreenPos).xyz;
	}

	if (cadran == 2) {
		return texture2D (reflectiveShadowMapPosMap, normalizedScreenPos).xyz;
	}

	if (cadran == 3) {
		return texture2D (reflectiveShadowMapNormalMap, normalizedScreenPos).xyz;
	}

	if (cadran == 4) {
		return texture2D (reflectiveShadowMapFluxMap, normalizedScreenPos).xyz;
	}
}

void main()
{
	out_color = CalcColor (CalcCadran ());

	// out_color = vec3 (1.0, 0.0, 0.0);
} 