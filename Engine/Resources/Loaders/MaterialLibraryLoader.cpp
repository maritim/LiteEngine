#include "MaterialLibraryLoader.h"

#include <string>
#include <limits>
#include <fstream>

#include "Utils/Files/FileSystem.h"

#include "Material/MaterialLibrary.h"
#include "Texture/Texture.h"
#include "Texture/CubeMap.h"

#include "Resources/Resources.h"

#include "Utils/Extensions/StringExtend.h"

#include "Core/Console/Console.h"

Object* MaterialLibraryLoader::Load(const std::string& filename)
{
	std::ifstream mtlFile (filename);

	std::string mtlName = filename;

	MaterialLibrary *materialLibrary = new MaterialLibrary();
 	Material* currentMaterial = NULL;

	if (mtlFile.is_open()) {
		std::string lineType;
		
		while (mtlFile >> lineType) {

			if (lineType == "#") {
				ProcessComment(mtlFile);
			}
			else if (lineType == "newmtl") {
				currentMaterial = SignalNewMaterial(mtlFile, filename, materialLibrary);
			}
			else if (lineType == "shaders") {
				ProcessShaders(mtlFile, currentMaterial, filename);
			}
			else if (lineType == "attribute") {
				// ProcessCustomAttributes(mtlFile, currentMaterial, filename);
			} else {
				ProcessDefaultAttributes(mtlFile, currentMaterial, filename, lineType);
			}
		}		
	}
	else {
		Console::LogError ("Unable to open material " + filename + " !");
	}

	materialLibrary->SetName (filename);

	return materialLibrary;
}

void MaterialLibraryLoader::ProcessComment(std::ifstream &file)
{
	file.ignore (std::numeric_limits<std::streamsize>::max(), '\n');	
}

Material* MaterialLibraryLoader::SignalNewMaterial(std::ifstream &file, std::string filename, MaterialLibrary* materialLibrary)
{
	std::string matName;
	file >> matName;

	Material *material = new Material();
	material->name = filename + "::" + matName;

	materialLibrary->AddMaterial(material);

	return material;
}

void MaterialLibraryLoader::ProcessShaders(std::ifstream &file, Material* currentMaterial, std::string filename)
{
	std::string vertexFile;
	std::string fragmentFile;

	file >> vertexFile >> fragmentFile;

	vertexFile = FileSystem::GetDirectory (filename) + vertexFile;
	fragmentFile = FileSystem::GetDirectory (filename) + fragmentFile;

	Resource<Shader> shader = Resources::LoadShader ({ vertexFile, fragmentFile });

	currentMaterial->shader = shader;
}

void MaterialLibraryLoader::ProcessCustomAttributes(std::ifstream &file, Material* currentMaterial, std::string filename)
{
	// Attribute attribute;
	// std::string attrType;
	// std::string attrName;
	// glm::vec3 values;

	// file >> attrType >> attrName;
	// if (attrType == "TEXTURE2D") {
	// 	attribute.type = Attribute::AttrType::ATTR_TEXTURE2D;

	// 	std::string attrTextureName;
	// 	std::getline (file, attrTextureName);

	// 	Extensions::StringExtend::Trim (attrTextureName);

	// 	attrTextureName = FileSystem::GetDirectory (filename) + attrTextureName;
	// 	attrTextureName = FileSystem::FormatFilename (attrTextureName);	

	// 	Resource<Texture> texture = Resources::LoadTexture (attrTextureName);

	// 	attribute.texture = texture;
	// }
	// else if (attrType == "TEXTURE2D_ATLAS") {
	// 	attribute.type = Attribute::AttrType::ATTR_TEXTURE2D_ATLAS;

	// 	std::string attrTextureName;
	// 	std::getline (file, attrTextureName);

	// 	Extensions::StringExtend::Trim (attrTextureName);

	// 	attrTextureName = FileSystem::GetDirectory (filename) + attrTextureName;
	// 	attrTextureName = FileSystem::FormatFilename (attrTextureName);	

	// 	Resource<Texture> texture = Resources::LoadTextureAtlas (attrTextureName);

	// 	attribute.texture = texture;		
	// }
	// else if (attrType == "TEXTURE_CUBE_MAP") {
	// 	attribute.type = Attribute::AttrType::ATTR_TEXTURE_CUBE;

	// 	std::vector<std::string> filenames;

	// 	std::string type;

	// 	file >> type;

	// 	if (type == "SKYBOX") {
	// 		values.x = 0;
	// 	}
	// 	else if (type == "CUSTOM") {
	// 		values.x = 1;

	// 		for (int i=0;i<6;i++) {
	// 			std::string attrTextureName;
	// 			file >> attrTextureName;

	// 			attrTextureName = FileSystem::GetDirectory (filename) + attrTextureName;

	// 			filenames.push_back (attrTextureName);
	// 		}

	// 		// CubeMap* cubeMap = Resources::LoadCubemap (filenames);
	// 		// TextureManager::Instance()->AddTexture (cubeMap);

	// 		// values.y = (float) cubeMap->GetGPUIndex ();
	// 	}
	// }
	// else if (attrType == "VEC3") {
	// 	attribute.type = Attribute::AttrType::ATTR_VEC3;

	// 	file >> values.x >> values.y >> values.z;
	// }
	// else if (attrType == "FLOAT") {
	// 	attribute.type = Attribute::AttrType::ATTR_FLOAT;

	// 	file >> values.x;
	// }
	// else {
	// 	attribute.type = Attribute::AttrType::ATTR_NONE;
	// }

	// attribute.name = attrName;
	// attribute.values = values;

	// currentMaterial->attributes.push_back (attribute);
}

void MaterialLibraryLoader::ProcessDefaultAttributes(std::ifstream &file, Material* currentMaterial, std::string filename, std::string fileType)
{
	if (fileType == "Ka")
	{
		float x, y, z;
		file >> x >> y >> z;

		currentMaterial->ambientColor = glm::vec3 (x, y, z);
	}
	else if (fileType == "Kd")
	{
		float x, y, z;
		file >> x >> y >> z;

		currentMaterial->diffuseColor = glm::vec3 (x, y, z);
	}
	else if (fileType == "Ks")
	{
		float x, y, z;
		file >> x >> y >> z;

		currentMaterial->specularColor = glm::vec3 (x, y, z);
	}
	else if (fileType == "Ke")
	{
		float x, y, z;
		file >> x >> y >> z;

		currentMaterial->emissiveColor = glm::vec3 (x, y, z);
	}
	else if (fileType == "Ns")
	{
		file >> currentMaterial->shininess;

		// Maybe do this in another way
		if (currentMaterial->shininess < 0.1) {
			Console::LogWarning ("The shininess of material is " + 
				std::to_string (currentMaterial->shininess) + 
				". Clamped to minimum value (0.100000).");
			currentMaterial->shininess = 0.1f;
		}
	}
	else if (fileType == "d" || fileType == "Tr")
	{
		float transparency = 0.0f;
		file >> transparency;

		if (fileType == "d") {
			transparency = 1.0 - transparency;
		}

		currentMaterial->transparency = transparency;
	}
	else if (fileType == "Ni")
	{
		file >> currentMaterial->refractiveIndex;
	}
	else if (fileType == "illum")
	{
		file >> currentMaterial->illum;
	}
	else if (fileType == "blending")
	{
		std::pair<int, int> blending;
		file >> blending.first >> blending.second;

		currentMaterial->blending = blending;
	}
	else if (fileType.substr (0, 4) == "map_")
	{
		std::string textureName;
		std::getline (file, textureName);

		Extensions::StringExtend::Trim (textureName);

		textureName = FileSystem::GetDirectory (filename) + textureName;
		textureName = FileSystem::FormatFilename (textureName);

		Resource<Texture> texture = Resources::LoadTexture (textureName);

		if (fileType == "map_Ka") {
			currentMaterial->ambientTexture = texture;
		}
		else if (fileType == "map_Kd") {
			currentMaterial->diffuseTexture = texture;
		}
		else if (fileType == "map_Ks") {
			currentMaterial->specularTexture = texture;
		}
		else if (fileType == "map_Ke") {
			currentMaterial->emissiveTexture = texture;
		}
		else if (fileType == "map_Ns") {
			currentMaterial->specularHighlight = texture;
		}
		else if (fileType == "map_d") {
			currentMaterial->alphaTexture = texture;
		}
		else if (fileType == "map_bump") {
			currentMaterial->bumpTexture = texture;
		}
	}
}
