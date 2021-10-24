#ifndef COMPONENTATTRIBUTELOADER_H
#define COMPONENTATTRIBUTELOADER_H

#include "Core/Interfaces/Object.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Core/Resources/Resource.h"

#include "Renderer/Render/Mesh/Model.h"

#include "Resources/Resources.h"

#include "Utils/Files/FileSystem.h"

#include "Utils/Extensions/StringExtend.h"

class ENGINE_API ComponentAttributeLoader : public Object
{
public:
	template<class T>
	static T Load (TiXmlElement* xmlElem) { return T (); }
	template<class T>
	static T GetValue (const std::string& value) { return T (); }
};

template <>
std::string ENGINE_API ComponentAttributeLoader::GetValue<std::string> (const std::string& value)
{
	return value;
}

template <>
int ENGINE_API ComponentAttributeLoader::GetValue<int> (const std::string& value)
{
	return std::stoi (value);
}

template <>
float ENGINE_API ComponentAttributeLoader::GetValue<float> (const std::string& value)
{
	return std::stof (value);
}

template <>
bool ENGINE_API ComponentAttributeLoader::GetValue<bool> (const std::string& value)
{
	return Extensions::StringExtend::ToBool (value);
}

template <>
glm::vec3 ENGINE_API ComponentAttributeLoader::Load<glm::vec3> (TiXmlElement* xmlElem)
{
	glm::vec3 result (0.0f);

	const char* x = xmlElem->Attribute ("x");
	const char* y = xmlElem->Attribute ("y");
	const char* z = xmlElem->Attribute ("z");

	if (x) {
		result.x = std::stof (x);
	}

	if (y) {
		result.y = std::stof (y);
	}

	if (z) {
		result.z = std::stof (z);
	}

	return result;
}

template <>
Resource<Model> ENGINE_API ComponentAttributeLoader::Load<Resource<Model>> (TiXmlElement* xmlElem)
{
	Resource<Model> model;

	std::string path = xmlElem->Attribute ("path");

	std::string extension = FileSystem::GetExtension(path);

	if (extension == ".anim") {
		model = Resources::LoadAnimatedModel (path);
	} else {
		model = Resources::LoadModel (path);
	}

	return model;
}

template <>
Resource<AudioClip> ENGINE_API ComponentAttributeLoader::Load<Resource<AudioClip>> (TiXmlElement* xmlElem)
{
	std::string path = xmlElem->Attribute ("path");

	Resource<AudioClip> audioClip = Resources::LoadAudioClip (path);

	return audioClip;
}

template <>
Color ENGINE_API ComponentAttributeLoader::Load<Color> (TiXmlElement* xmlElem)
{
	Color color = Color::Black;

	const char* r = xmlElem->Attribute ("r");
	const char* g = xmlElem->Attribute ("g");
	const char* b = xmlElem->Attribute ("b");
	const char* a = xmlElem->Attribute ("a");

	if (r) {
		color.r = std::stoi (r);
	}

	if (g) {
		color.g = std::stoi (g);
	}

	if (b) {
		color.b = std::stoi (b);
	}

	if (a) {
		color.a = std::stoi (a);
	}

	return color;
}

#include "Components/Lighting/LightComponent.h"

template <>
LightComponent::ShadowInformation ENGINE_API ComponentAttributeLoader::Load<LightComponent::ShadowInformation> (TiXmlElement* xmlElem)
{
	LightComponent::ShadowInformation shadowInformation;

	const char* cascadesCount = xmlElem->Attribute ("cascadesCount");
	const char* bias = xmlElem->Attribute ("bias");

	if (cascadesCount != nullptr) {
		shadowInformation.cascadesCount = std::stoi (cascadesCount);
	}

	if (bias != nullptr) {
		shadowInformation.bias = std::stof (bias);
	}

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Resolution") {
			const char* width = content->Attribute ("width");
			const char* height = content->Attribute ("height");

			if (width) {
				shadowInformation.resolution.x = std::stoi (width);
			}

			if (height) {
				shadowInformation.resolution.y = std::stoi (height);
			}
		}

		content = content->NextSiblingElement ();
	}

	return shadowInformation;
}

template <>
Resource<Font> ENGINE_API ComponentAttributeLoader::Load<Resource<Font>> (TiXmlElement* xmlElem)
{
	std::string path = xmlElem->Attribute ("path");

	Resource<Font> font = Resources::LoadBitmapFont (path);

	return font;
}

#endif