#include "Material.h"

#include "Core/Console/Console.h"

Material::Material(void) :
	name(""),
	ambientColor(0.2, 0.2, 0.2),
	diffuseColor(0.8, 0.8, 0.8),
	specularColor (1.0, 1.0, 1.0),
	shininess(32),
	transparency (0.0),
	refractiveIndex (1.0),
	illum(0),
	blending (770, 771),
	ambientTexture(nullptr),
	diffuseTexture (nullptr),
	specularTexture (nullptr),
	specularHighlight(nullptr),
	alphaTexture (nullptr),
	bumpTexture (nullptr),
	cubeTexture (nullptr),
	shaderName ("")
{
	// Documentation: 
	// http://stackoverflow.com/questions/10181201/opengl-light-changes-ambient-to-diffuse-or-specular-works-but-not-the-opposite
}

Material::Material(const Material& other) :
	name (other.name),
	ambientColor (other.ambientColor),
	diffuseColor (other.diffuseColor),
	specularColor (other.specularColor),
	shininess (other.shininess),
	transparency (other.transparency),
	illum (other.illum),
	blending (other.blending),
	ambientTexture (other.ambientTexture),
	diffuseTexture (other.diffuseTexture),
	specularTexture (other.specularTexture),
	specularHighlight (other.specularHighlight),
	alphaTexture (other.alphaTexture),
	bumpTexture (other.bumpTexture),
	cubeTexture (other.cubeTexture),
	shaderName (other.shaderName)
{

}

Material::~Material ()
{
}
