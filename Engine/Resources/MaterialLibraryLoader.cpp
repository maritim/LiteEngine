#include "MaterialLibraryLoader.h"

#include <string>
#include <limits>
#include <fstream>

#include "Utils/Files/FileSystem.h"

#include "Material/MaterialLibrary.h"
#include "Texture/Texture.h"
#include "Texture/CubeMap.h"

#include "Resources.h"

#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

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
				ProcessCustomAttributes(mtlFile, currentMaterial, filename);
			} else {
				ProcessDefaultAttributes(mtlFile, currentMaterial, filename, lineType);
			}
		}		
	}
	else {
		Console::LogError ("Unable to open material " + filename + " !");
	}

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

	std::string shaderName = vertexFile;

	Console::Log ("Shader name: " + shaderName);

	ShaderManager::Instance ()->AddShader (shaderName, vertexFile, fragmentFile);

	currentMaterial->shaderName = shaderName;
}

void MaterialLibraryLoader::ProcessCustomAttributes(std::ifstream &file, Material* currentMaterial, std::string filename)
{
	Attribute attribute;
	std::string attrType;
	std::string attrName;
	glm::vec3 values;

	file >> attrType >> attrName;
	if (attrType == "TEXTURE2D") {
		attribute.type = Attribute::AttrType::ATTR_TEXTURE2D;

		std::string attrTextureName;
		std::getline (file, attrTextureName);

		Extensions::StringExtend::Trim (attrTextureName);

		attrTextureName = FileSystem::GetDirectory (filename) + attrTextureName;
		attrTextureName = FileSystem::FormatFilename (attrTextureName);	

		Texture* texture = TextureManager::Instance ()->GetTexture (attrTextureName);

		if (texture == nullptr) {
			texture = Resources::LoadTexture (attrTextureName);
			TextureManager::Instance ()->AddTexture (texture);
		}

		attribute.valueName = attrTextureName;
	}
	else if (attrType == "TEXTURE2D_ATLAS") {
		attribute.type = Attribute::AttrType::ATTR_TEXTURE2D_ATLAS;

		std::string attrTextureName;
		std::getline (file, attrTextureName);

		Extensions::StringExtend::Trim (attrTextureName);

		attrTextureName = FileSystem::GetDirectory (filename) + attrTextureName;
		attrTextureName = FileSystem::FormatFilename (attrTextureName);	

		Texture* texture = TextureManager::Instance ()->GetTexture (attrTextureName);
		TextureAtlas* textureAtlas = dynamic_cast<TextureAtlas*> (texture);

		if (textureAtlas == nullptr) {
			textureAtlas = Resources::LoadTextureAtlas (attrTextureName);
			TextureManager::Instance ()->AddTexture (textureAtlas);
		}

		attribute.valueName = attrTextureName;		
	}
	else if (attrType == "TEXTURE_CUBE_MAP") {
		attribute.type = Attribute::AttrType::ATTR_TEXTURE_CUBE;

		std::vector<std::string> filenames;

		std::string type;

		file >> type;

		if (type == "SKYBOX") {
			values.x = 0;
		}
		else if (type == "CUSTOM") {
			values.x = 1;

			for (int i=0;i<6;i++) {
				std::string attrTextureName;
				file >> attrTextureName;

				attrTextureName = FileSystem::GetDirectory (filename) + attrTextureName;

				filenames.push_back (attrTextureName);
			}

			CubeMap* cubeMap = Resources::LoadCubemap (filenames);
			TextureManager::Instance()->AddTexture (cubeMap);

			values.y = (float) cubeMap->GetGPUIndex ();
		}
	}
	else if (attrType == "VEC3") {
		attribute.type = Attribute::AttrType::ATTR_VEC3;

		file >> values.x >> values.y >> values.z;
	}
	else if (attrType == "FLOAT") {
		attribute.type = Attribute::AttrType::ATTR_FLOAT;

		file >> values.x;
	}
	else {
		attribute.type = Attribute::AttrType::ATTR_NONE;
	}

	attribute.name = attrName;
	attribute.values = values;

	currentMaterial->attributes.push_back (attribute);
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
		float transparency = 1.0f;
		file >> transparency;

		if (fileType == "Tr") {
			transparency = 1.0f - transparency;
		}

		currentMaterial->transparency = transparency;
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

		Texture* texture = TextureManager::Instance ()->GetTexture (textureName);

		if (texture == nullptr) {
			texture = Resources::LoadTexture (textureName);
			TextureManager::Instance ()->AddTexture (texture);
		}

		unsigned int textureId = texture->GetGPUIndex ();

		if (fileType == "map_Ka") {
			currentMaterial->ambientTexture = textureId;
		}
		else if (fileType == "map_Kd") {
			currentMaterial->diffuseTexture = textureId;
		}
		else if (fileType == "map_Ks") {
			currentMaterial->specularTexture = textureId;
		}
		else if (fileType == "map_Ns") {
			currentMaterial->specularHighlight = textureId;
		}
		else if (fileType == "map_d") {
			currentMaterial->alphaTexture = textureId;
		}
	}
}
