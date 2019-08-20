uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform vec3 attenuationComp;

vec3 CalcDirectionalPhongDiffuseColor (vec3 viewNormal, vec3 diffuseColor)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (vec3 (viewMatrix * vec4 (lightPosition, 0)));

	// Diffuse contribution
	float dCont = max (dot (viewNormal, lightDirection), 0.0);

	// Attenuation is 1.0 for Directional Lights
	vec3 diffuse = lightColor * diffuseColor * dCont * lightIntensity;

	return diffuse;
}

vec3 CalcDirectionalPhongSpecularColor (vec3 viewPosition, vec3 viewNormal, vec3 specularColor, float shininess)
{
	// The position is also a direction for Directional Lights
	vec3 lightDirection = normalize (vec3 (viewMatrix * vec4 (lightPosition, 0)));

	vec3 surface2view = normalize (-viewPosition);
	vec3 reflection = reflect (-lightDirection, viewNormal);

	// Specular contribution
	float sCont = pow (max (dot (surface2view, reflection), 0.0), shininess);

	vec3 specular = lightColor * specularColor * sCont * lightIntensity;

	return specular;
}
