vec3 RGBToYCoCg (const in vec3 rgbColor)
{
	return rgbColor;
	const mat3 colorTransformMatrix = mat3 (
		0.25f, 0.5f, -0.25f,
		0.5f, 0.0f, 0.5f,
		0.25f, -0.5f, -0.25f
	);

	return colorTransformMatrix * rgbColor;
}

vec3 YCoCgToRGB (const in vec3 YCoCgColor)
{
	return YCoCgColor;
	const mat3 colorTransformMatrix = mat3 (
		1.f, 1.f, 1.f,
		1.f, 0.f, -1.f,
		-1.f, 1.f, -1.f
	);

	return colorTransformMatrix * YCoCgColor;
}

float CalcLuminance (const in vec3 color)
{
	return dot (color, vec3 (0.2126, 0.7152, 0.0722));
}
