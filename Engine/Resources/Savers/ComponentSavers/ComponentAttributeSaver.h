#ifndef COMPONENTATTRIBUTESAVER_H
#define COMPONENTATTRIBUTESAVER_H

#include "Core/Interfaces/Object.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Core/Resources/Resource.h"

#include "Mesh/Model.h"

#include "Resources/Resources.h"

#include "Utils/Files/FileSystem.h"

#include "Utils/Extensions/StringExtend.h"

class ENGINE_API ComponentAttributeSaver : public Object
{
public:
	template<class T>
	static void Save (TiXmlElement* xmlElem, const T& value, const std::string& name) { }
};

template <>
void ENGINE_API ComponentAttributeSaver::Save<std::string> (
	TiXmlElement* xmlElem, const std::string& value, const std::string& name)
{
	xmlElem->SetAttribute (name.c_str (), value.c_str ());
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<int> (
	TiXmlElement* xmlElem, const int& value, const std::string& name)
{
	xmlElem->SetAttribute (name.c_str (), std::to_string (value).c_str ());
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<float> (
	TiXmlElement* xmlElem, const float& value, const std::string& name)
{
	xmlElem->SetAttribute (name.c_str (), std::to_string (value).c_str ());
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<bool> (
	TiXmlElement* xmlElem, const bool& value, const std::string& name)
{
	xmlElem->SetAttribute (name.c_str (), value ? "true" : "false");
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<Resource<Model>> (
	TiXmlElement* xmlElem, const Resource<Model>& value, const std::string& name)
{
	TiXmlElement* componentElem = new TiXmlElement (name.c_str ());

	componentElem->SetAttribute ("path", value.GetPath ().c_str ());

    xmlElem->LinkEndChild(componentElem);
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<Resource<AudioClip>> (
	TiXmlElement* xmlElem, const Resource<AudioClip>& value, const std::string& name)
{
	TiXmlElement* componentElem = new TiXmlElement (name.c_str ());

	componentElem->SetAttribute ("path", value.GetPath ().c_str ());

    xmlElem->LinkEndChild(componentElem);
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<glm::vec3> (
	TiXmlElement* xmlElem, const glm::vec3& value, const std::string& name)
{
	TiXmlElement* componentElem = new TiXmlElement (name.c_str ());

	componentElem->SetAttribute ("x", std::to_string (value.x).c_str ());
	componentElem->SetAttribute ("y", std::to_string (value.y).c_str ());
	componentElem->SetAttribute ("z", std::to_string (value.z).c_str ());

    xmlElem->LinkEndChild(componentElem);
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<Color> (
	TiXmlElement* xmlElem, const Color& value, const std::string& name)
{
	TiXmlElement* componentElem = new TiXmlElement (name.c_str ());

	componentElem->SetAttribute ("r", std::to_string (value.r).c_str ());
	componentElem->SetAttribute ("g", std::to_string (value.g).c_str ());
	componentElem->SetAttribute ("b", std::to_string (value.b).c_str ());
	componentElem->SetAttribute ("a", std::to_string (value.a).c_str ());

    xmlElem->LinkEndChild(componentElem);
}

#include "Components/Lighting/LightComponent.h"

template <>
void ENGINE_API ComponentAttributeSaver::Save<LightComponent::ShadowInformation> (
	TiXmlElement* xmlElem, const LightComponent::ShadowInformation& value, const std::string& name)
{
	TiXmlElement* componentElem = new TiXmlElement (name.c_str ());

	componentElem->SetAttribute ("cascadesCount", std::to_string (value.cascadesCount).c_str ());
	componentElem->SetAttribute ("bias", std::to_string (value.bias).c_str ());

	TiXmlElement* resolutionElem = new TiXmlElement ("Resolution");

	resolutionElem->SetAttribute ("width", std::to_string (value.resolution.x).c_str ());
	resolutionElem->SetAttribute ("height", std::to_string (value.resolution.y).c_str ());

    componentElem->LinkEndChild(resolutionElem);

    xmlElem->LinkEndChild(componentElem);
}

template <>
void ENGINE_API ComponentAttributeSaver::Save<Resource<Font>> (
	TiXmlElement* xmlElem, const Resource<Font>& value, const std::string& name)
{
	TiXmlElement* componentElem = new TiXmlElement (name.c_str ());

	componentElem->SetAttribute ("path", value.GetPath ().c_str ());

    xmlElem->LinkEndChild(componentElem);
}

#endif