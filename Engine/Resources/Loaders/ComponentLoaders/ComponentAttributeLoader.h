#ifndef COMPONENTATTRIBUTELOADER_H
#define COMPONENTATTRIBUTELOADER_H

#include "Core/Interfaces/Object.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Core/Resources/Resource.h"

#include "Mesh/Model.h"

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

#include "Systems/Collision/BoxCollider.h"
#include "Systems/Collision/SphereCollider.h"
#include "Systems/Collision/CylinderCollider.h"
#include "Systems/Collision/CapsuleCollider.h"
#include "Systems/Collision/MeshCollider.h"

template <>
BulletCollider* ENGINE_API ComponentAttributeLoader::Load<BulletCollider*> (TiXmlElement* xmlElem)
{
	BulletCollider* collider = nullptr;

	std::string shape = xmlElem->Attribute ("shape");

	if (shape == "Box") {
		const char* modelPath = xmlElem->Attribute ("modelPath");

		if (modelPath != nullptr) {
			collider = new BoxCollider (Resources::LoadModel (modelPath));
		}

		if (modelPath == nullptr) {
			std::string x = xmlElem->Attribute ("x");
			std::string y = xmlElem->Attribute ("y");
			std::string z = xmlElem->Attribute ("z");

			collider = new BoxCollider (glm::vec3 (std::stof (x), std::stof (y), std::stof (z)));
		}
	}
	else if (shape == "Sphere") {
		const char* modelPath = xmlElem->Attribute ("modelPath");

		if (modelPath != nullptr) {
			collider = new SphereCollider (Resources::LoadModel (modelPath));
		}

		if (modelPath == nullptr) {
			std::string radius = xmlElem->Attribute ("radius");

			collider = new SphereCollider (std::stof (radius));
		}
	}
	else if (shape == "Cylinder") {
		const char* modelPath = xmlElem->Attribute ("modelPath");

		if (modelPath != nullptr) {
			collider = new CylinderCollider (Resources::LoadModel (modelPath));
		}

		if (modelPath == nullptr) {
			std::string radius = xmlElem->Attribute ("radius");
			std::string height = xmlElem->Attribute ("height");

			collider = new CylinderCollider (std::stof (radius), std::stof (height));
		}
	}
	else if (shape == "Capsule") {
		const char* modelPath = xmlElem->Attribute ("modelPath");

		if (modelPath != nullptr) {
			collider = new CapsuleCollider (Resources::LoadModel (modelPath));
		}

		if (modelPath == nullptr) {
			std::string radius = xmlElem->Attribute ("radius");
			std::string height = xmlElem->Attribute ("height");

			collider = new CapsuleCollider (std::stof (radius), std::stof (height));
		}
	}
	else if (shape == "Mesh") {
		const char* modelPath = xmlElem->Attribute ("modelPath");

		if (modelPath != nullptr) {
			collider = new MeshCollider (Resources::LoadModel (modelPath));
		}
	}

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Offset") {
			glm::vec3 vec (0.0f);

			const char* x = content->Attribute ("x");
			const char* y = content->Attribute ("y");
			const char* z = content->Attribute ("z");

			if (x) {
				vec.x = std::stof (x);
			}

			if (y) {
				vec.y = std::stof (y);
			}

			if (z) {
				vec.z = std::stof (z);
			}

			collider->SetOffset (vec);
		}

		content = content->NextSiblingElement ();
	}

	return collider;
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

#include "Components/LightComponent.h"

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