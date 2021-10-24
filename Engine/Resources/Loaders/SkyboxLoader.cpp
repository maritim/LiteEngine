#include "SkyboxLoader.h"

#include <string>

#include "Renderer/Render/Texture/CubeMapFace.h"

#include "Utils/Color/Color.h"
#include "Utils/Files/FileSystem.h"

#include "Resources/Resources.h"

Object* SkyboxLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		return NULL;
	}

	TiXmlElement* root = doc.FirstChildElement ("Skybox");

	if (root == NULL) {
		return NULL;
	}

	Skybox* skybox = new Skybox ();

	skybox->SetName (filename);

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Faces") {
			ProcessFaces (content, skybox, filename);
		}
		else if (name == "TintColor") {
			ProcessTintColor (content, skybox);
		}
		else if (name == "Exposure") {
			ProcessExposure (content, skybox);
		}
		else if (name == "Rotation") {
			ProcessRotation (content, skybox);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return skybox;
}

void SkyboxLoader::ProcessFaces (TiXmlElement* xmlElem, Skybox* skybox, const std::string& filename)
{
	std::vector<std::string> facePaths; 
	facePaths.resize (6, "");

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Right") {
			facePaths [CubeMapFace::CUBE_MAP_FACE_RIGHT] = 
				FileSystem::GetDirectory (filename) + content->Attribute ("path");
		}
		else if (name == "Left") {
			facePaths [CubeMapFace::CUBE_MAP_FACE_LEFT] = 
				FileSystem::GetDirectory (filename) + content->Attribute ("path");
		}
		else if (name == "Up") {
			facePaths [CubeMapFace::CUBE_MAP_FACE_UP] = 
				FileSystem::GetDirectory (filename) + content->Attribute ("path");
		}
		else if (name == "Down") {
			facePaths [CubeMapFace::CUBE_MAP_FACE_DOWN] = 
				FileSystem::GetDirectory (filename) + content->Attribute ("path");
		}
		else if (name == "Back") {
			facePaths [CubeMapFace::CUBE_MAP_FACE_BACK] = 
				FileSystem::GetDirectory (filename) + content->Attribute ("path");
		}
		else if (name == "Front") {
			facePaths [CubeMapFace::CUBE_MAP_FACE_FRONT] = 
				FileSystem::GetDirectory (filename) + content->Attribute ("path");
		}

		content = content->NextSiblingElement ();
	}

	Resource<Texture> cubeMap = Resources::LoadCubemap (facePaths);
	skybox->SetCubeMap (cubeMap);
}

void SkyboxLoader::ProcessTintColor (TiXmlElement* xmlElem, Skybox* skybox)
{
	const char* r = xmlElem->Attribute ("red");
	const char* g = xmlElem->Attribute ("green");
	const char* b = xmlElem->Attribute ("blue");
	const char* a = xmlElem->Attribute ("alpha");

	Color tintColor;

	if (r) {
		tintColor.r = (char) std::stoi (r);
	}

	if (g) {
		tintColor.g = (char) std::stoi (g);
	}

	if (b) {
		tintColor.b = (char) std::stoi (b);
	}

	if (a) {
		tintColor.a = (char) std::stoi (a);
	}

	skybox->SetTintColor (tintColor);
}

void SkyboxLoader::ProcessExposure (TiXmlElement* xmlElem, Skybox* skybox)
{
	const char* brightness = xmlElem->Attribute ("brightness");

	if (brightness) {
		skybox->SetBrightness (std::stof (brightness));
	}
}

void SkyboxLoader::ProcessRotation (TiXmlElement* xmlElem, Skybox* skybox)
{
	const char* angle = xmlElem->Attribute ("angularVelocity");

	if (angle) {
		skybox->SetAngularVelocity (std::stof (angle));
	}
}