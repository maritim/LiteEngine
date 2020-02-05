/*************************************************************************** 
 Material
 ***************************************************************************/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Core/Interfaces/Object.h"

#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "Core/Resources/Resource.h"
#include "Texture/Texture.h"
#include "Shader/Shader.h"

class Material : public Object
{
public:
	std::string name;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	float transparency;
	float refractiveIndex;
	int illum;
	std::pair<int, int> blending;
	Resource<Texture> ambientTexture;
	Resource<Texture> diffuseTexture;
	Resource<Texture> specularTexture;
	Resource<Texture> specularHighlight;
	Resource<Texture> alphaTexture;
	Resource<Texture> bumpTexture;
	Resource<Texture> cubeTexture;
	Resource<Shader> shader;

public:
	Material(void);
	Material(const Material& other);
	~Material();
};

#endif