#include "Material.h"

#include "Core/Console/Console.h"

Material::Material(void) :
	name(""),
	ambientColor(0.2, 0.2, 0.2),
	diffuseColor(0.8, 0.8, 0.8),
	specularColor(0.0, 0.0, 0.0),
	shininess(32),
	transparency (1.0),
	illum(0),
	blending (770, 771),
	ambientTexture(0),
	diffuseTexture (0),
	specularTexture (0),
	specularHighlight(0),
	alphaTexture (0),
	bumpTexture (0),
	cubeTexture (0),
	shaderName (""),
	attributes ()
{
	// Documentation: 
	// http://stackoverflow.com/questions/10181201/opengl-light-changes-ambient-to-diffuse-or-specular-works-but-not-the-opposite
}

Material::Material(const std::string& na, Vector3 aC, Vector3 dC, Vector3 sC, float ns, float tr, int lum, unsigned int tex,
	const std::pair<int, int>& blend, unsigned int bumpTex, unsigned int cubeTex, const std::string& shaderNam, 
	const std::vector<Attribute>& attr) :
	name(na),
	ambientColor(aC),
	diffuseColor(dC),
	specularColor(sC),
	shininess(ns),
	transparency (tr),
	illum(lum),
	blending (blend),
	ambientTexture(tex),
	diffuseTexture(tex),
	specularTexture(tex),
	alphaTexture (0),
	specularHighlight(0),
	bumpTexture (bumpTex),
	cubeTexture (cubeTex),
	shaderName (shaderNam),
	attributes (attr)
{
	// Nothing
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
	shaderName (other.shaderName),
	attributes (other.attributes)
{

}

Material::~Material ()
{
}

Attribute Material::GetAttribute (Attribute::AttrType type) const
{
	for (std::size_t i=0;i<attributes.size ();i++) {
		if (attributes [i].type == type) {
			return attributes [i];
		}
	}

	Console::LogWarning ("In material " + name + " is no attribute of type " + 
		std::to_string (type));

	Attribute invalid;
	return invalid;
}