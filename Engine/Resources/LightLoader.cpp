#include "LightLoader.h"

Object* LightLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		return NULL;
	}

	TiXmlElement* root = doc.FirstChildElement ("Light");

	if (root == NULL) {
		return NULL;
	}

	Light* light = new Light ();

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Type") {
			ProcessType (content, light);
		}
		else if (name == "Color") {
			ProcessColor (content, light);
		}
		else if (name == "SpecularColor") {
			ProcessSpecularColor (content, light);
		}
		else if (name == "Attenuation") {
			ProcessAttenuation (content, light);
		}
		else if (name == "Spot") {
			ProcessSpotLight (content, light);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return light;
}

void LightLoader::ProcessType (TiXmlElement* xmlElem, Light* light)
{
	std::string type = xmlElem->Attribute ("type");

	if (type == "DIRECTIONAL_LIGHT") {
		light->SetType (Light::Type::DIRECTIONAL_LIGHT);
	}
	else if (type == "POINT_LIGHT") {
		light->SetType (Light::Type::POINT_LIGHT);
	}
	else if (type == "SPOT_LIGHT") {
		light->SetType (Light::Type::SPOT_LIGHT);
	}
}

void LightLoader::ProcessColor (TiXmlElement* xmlElem, Light* light)
{
	Color color = GetColor (xmlElem);

	light->SetColor (color);
}

void LightLoader::ProcessSpecularColor (TiXmlElement* xmlElem, Light* light)
{
	Color specularColor = GetColor (xmlElem);

	light->SetSpecularColor (specularColor);
}

void LightLoader::ProcessAttenuation (TiXmlElement* xmlElem, Light* light)
{
	const char* constant = xmlElem->Attribute ("constant");
	const char* linear = xmlElem->Attribute ("linear");
	const char* quadratic = xmlElem->Attribute ("quadratic");

	if (constant) {
		light->SetConstantAttenuation (std::stof (constant));
	}

	if (linear) {
		light->SetLinearAttenuation (std::stof (linear));
	}

	if (quadratic) {
		light->SetQuadraticAttenuation (std::stof (quadratic));
	}
}

void LightLoader::ProcessSpotLight (TiXmlElement* xmlElem, Light* light)
{
	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Cutoff") {
			ProcessSpotCutoff (content, light);
		}
		else if (name == "Exponent") {
			ProcessSpotExponent (content, light);
		}
		else if (name == "Direction") {
			ProcessSpotDirection (content, light);
		}

		content = content->NextSiblingElement ();
	}
}

void LightLoader::ProcessSpotCutoff (TiXmlElement* xmlElem, Light* light)
{
	std::string value = xmlElem->Attribute ("value");

	light->SetSpotCutoff (std::stof (value));
}

void LightLoader::ProcessSpotExponent (TiXmlElement* xmlElem, Light* light)
{
	std::string value = xmlElem->Attribute ("value");

	light->SetSpotExponent (std::stof (value));
}

void LightLoader::ProcessSpotDirection (TiXmlElement* xmlElem, Light* light)
{
	Vector3 direction = Vector3::Zero;

	const char* x = xmlElem->Attribute ("x");
	const char* y = xmlElem->Attribute ("y");
	const char* z = xmlElem->Attribute ("z");

	if (x) {
		direction.x = std::stof (x);
	}

	if (y) {
		direction.y = std::stof (y);
	}

	if (z) {
		direction.z = std::stof (z);
	}

	light->SetSpotDirection (direction);
}

Color LightLoader::GetColor (TiXmlElement* xmlElem)
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