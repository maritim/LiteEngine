/*************************************************************************** 
 Material
 ***************************************************************************/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <vector>

#include "Core/Math/glm/vec3.hpp"

struct Attribute
{
	enum AttrType { ATTR_FLOAT = 0, ATTR_VEC3, ATTR_TEXTURE2D, ATTR_TEXTURE2D_ATLAS, ATTR_TEXTURE_CUBE, ATTR_NONE };

	std::string name;
	AttrType type;
	glm::vec3 values;
	std::string valueName;

	Attribute ();
};

class Material : public Object
{
public:
	std::string name;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	float transparency;
	int illum;
	std::pair<int, int> blending;
	unsigned int ambientTexture;
	unsigned int diffuseTexture;
	unsigned int specularTexture;
	unsigned int specularHighlight;
	unsigned int alphaTexture;
	unsigned int bumpTexture;
	unsigned int cubeTexture;
	std::string shaderName;

	std::vector<Attribute> attributes;

public:
	Material(void);
	Material(const std::string& na, glm::vec3 aC, glm::vec3 dC, glm::vec3 sC, float ns, float tr, int ill, unsigned int tex,
		const std::pair<int, int>& blend, unsigned int bumpTex, unsigned int cubeTex, const std::string& shaderNam, 
		const std::vector<Attribute>& attr);
	Material(const Material& other);
	~Material();

	std::vector<Attribute> GetAttributes (Attribute::AttrType type) const;
	Attribute GetAttribute (Attribute::AttrType type) const;
};

#endif