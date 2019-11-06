#include "LightLoader.h"

#include "Utils/Extensions/StringExtend.h"

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

	Light* light = GetLight (root);

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Color") {
			ProcessColor (content, light);
		}
		else if (name == "Intensity") {
			ProcessIntensity (content, light);
		}
		else if (name == "Point") {
			ProcessPointLight (content, light);
		}
		else if (name == "Spot") {
			ProcessSpotLight (content, light);
		}
		else if (name == "Shadows") {
			ProcessShadows (content, light);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return light;
}

Light* LightLoader::GetLight (TiXmlElement* xmlElem)
{
	std::string type = xmlElem->Attribute ("type");

	if (type == "DIRECTIONAL_LIGHT") {
		return new DirectionalLight ();
	}
	else if (type == "POINT_LIGHT") {
		return new PointLight ();
	}
	else if (type == "SPOT_LIGHT") {
		return new SpotLight ();
	}
	else if (type == "AMBIENT_LIGHT") {
		return new AmbientLight ();
	}

	return nullptr;
}

void LightLoader::ProcessColor (TiXmlElement* xmlElem, Light* light)
{
	Color color = GetColor (xmlElem);

	light->SetColor (color);
}

void LightLoader::ProcessIntensity (TiXmlElement* xmlElem, Light* light)
{
	const char* value = xmlElem->Attribute ("value");

	if (value) {
		light->SetIntensity (std::stof (value));
	}
}

void LightLoader::ProcessShadows (TiXmlElement* xmlElem, Light* light)
{
	const char* casting = xmlElem->Attribute ("casting");

	if (casting) {
		light->SetShadowCasting (Extensions::StringExtend::ToBool (casting));
	}

	Light::Shadow shadow;

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Resolution") {
			ProcessShadowResolution (content, shadow);
		}
		else if (name == "Cascades") {
			ProcessShadowCascades (content, shadow);
		}
		else if (name == "Bias") {
			ProcessShadowBias (content, shadow);
		}

		content = content->NextSiblingElement ();
	}

	light->SetShadow (shadow);
}

void LightLoader::ProcessPointLight (TiXmlElement* xmlElem, Light* light)
{
	PointLight* pointLight = dynamic_cast<PointLight*> (light);

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Range") {
			ProcessRange (content, pointLight);
		}

		content = content->NextSiblingElement ();
	}
}

void LightLoader::ProcessSpotLight (TiXmlElement* xmlElem, Light* light)
{
	SpotLight* spotLight = dynamic_cast<SpotLight*> (light);

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content)
	{
		std::string name = content->Value ();

		if (name == "Cutoff") {
			ProcessSpotCutoff (content, spotLight);
		}
		else if (name == "Exponent") {
			ProcessSpotExponent (content, spotLight);
		}
		else if (name == "Direction") {
			ProcessSpotDirection (content, spotLight);
		}

		content = content->NextSiblingElement ();
	}
}

void LightLoader::ProcessRange (TiXmlElement* xmlElem, PointLight* light)
{
	const char* value = xmlElem->Attribute ("value");

	light->SetRange (std::stof (value));
}

void LightLoader::ProcessSpotCutoff (TiXmlElement* xmlElem, SpotLight* light)
{
	std::string value = xmlElem->Attribute ("value");

	light->SetSpotCutoff (std::stof (value));
}

void LightLoader::ProcessSpotExponent (TiXmlElement* xmlElem, SpotLight* light)
{
	std::string value = xmlElem->Attribute ("value");

	light->SetSpotExponent (std::stof (value));
}

void LightLoader::ProcessSpotDirection (TiXmlElement* xmlElem, SpotLight* light)
{
	glm::vec3 direction;

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

void LightLoader::ProcessShadowResolution (TiXmlElement* xmlElem, Light::Shadow& shadow)
{
	const char* width = xmlElem->Attribute ("width");
	const char* height = xmlElem->Attribute ("height");

	if (width) {
		shadow.resolution.x = std::stoi (width);
	}

	if (height) {
		shadow.resolution.y = std::stoi (height);
	}
}

void LightLoader::ProcessShadowCascades (TiXmlElement* xmlElem, Light::Shadow& shadow)
{
	const char* count = xmlElem->Attribute ("count");

	if (count) {
		shadow.cascadesCount = std::stoi (count);
	}
}

void LightLoader::ProcessShadowBias (TiXmlElement* xmlElem, Light::Shadow& shadow)
{
	const char* value = xmlElem->Attribute ("value");

	if (value) {
		shadow.bias = std::stof (value);
	}
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