#version 120

varying vec3 position;
varying vec3 normal; 
varying vec3 lightpos;

void main()
{
	vec3 ambient = vec3 (gl_FrontMaterial.ambient * gl_LightSource [0].ambient);

	// vector which point from pixel to light
	vec3 surface2light = normalize(lightpos - position );

	// Renormalise normal vector
	vec3 norm = normalize (normal);

	// diffuse contribution
	float dCont = max (dot (norm, surface2light), 0.0);

	vec3 diffuse = vec3 (gl_FrontMaterial.diffuse * gl_LightSource [0].diffuse * dCont);

	float dist = length (position - lightpos);
	float att = 1.0 / (0.8 + 0.1 * dist + 0.01 * dist * dist);

	vec3 surface2view = normalize (-position);
	// the reflection from light vector from normal
	vec3 reflection = -reflect (surface2light, norm);

	float sCont = max (dot (surface2view, reflection), 0.0);
	sCont = pow (sCont, gl_FrontMaterial.shininess);

	vec3 specular = vec3 (gl_FrontMaterial.specular * gl_LightSource [0].specular * sCont);

	gl_FragColor = vec4 ((ambient + diffuse + specular) * att, 1.0);
}