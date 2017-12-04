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
	vec2 normalizedScreenPos = geom_ScreenCoordinates;

	if (cadran == 1) {
		normalizedScreenPos.x = 1 + normalizedScreenPos.x;
		normalizedScreenPos.y = 1 + normalizedScreenPos.y;
		return texture (reflectiveShadowMap, normalizedScreenPos).xyz;
	}

	if (cadran == 2) {
		normalizedScreenPos.x = 1 + normalizedScreenPos.x;
		return texture (reflectiveShadowMapPosMap, normalizedScreenPos).xyz;
	}

	if (cadran == 3) {
		normalizedScreenPos.y = 1 + normalizedScreenPos.y;
		return texture (reflectiveShadowMapNormalMap, normalizedScreenPos).xyz;
	}

	if (cadran == 4) {
		return texture (reflectiveShadowMapFluxMap, normalizedScreenPos).xyz;
	}
}

void main()
{
	out_color = CalcColor (CalcCadran ());
} 