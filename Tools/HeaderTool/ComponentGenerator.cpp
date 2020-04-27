#include "ComponentGenerator.h"

#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim1(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return ch != '_';
    }));
}

// trim from end (in place)
static inline void rtrim1(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return ch != '_';
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim1(std::string &s) {
    ltrim1(s);
    rtrim1(s);
}

// trim from start (copying)
static inline std::string ltrim(std::string s) {
    ltrim1(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim(std::string s) {
    rtrim1(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim(std::string s) {
    trim1(s);
    return s;
}

// trim from both ends (copying)
static inline std::string toupper(std::string s) {
	for (auto & c: s) c = toupper(c);
    return s;
}

static inline std::string capitalise (std::string s) {
	s [0] = toupper (s [0]);
	return s;
}

std::string ComponentGenerator::Generate (const ClassType* classType)
{
	std::string text;

	text = "#include \"" + classType->Path + "\"\n"
			"class ENGINE_API HT" + classType->TypeName + " : public " + classType->TypeName + ", public EditorComponent\n"
			"{\n";

	text += GenerateLoader (classType);
	text += GenerateSaver (classType);
	text += GenerateWidget (classType);

	text += "};\n"
			"REGISTER_COMPONENT(HT" + classType->TypeName + ")\n\n";

	return text;
}

std::string ComponentGenerator::GenerateLoader (const ClassType* classType)
{
	std::string text;

	text = "public:\n\tvoid Load (TiXmlElement* xmlElem)\n\t{\n";

	text += GenerateAttributesLoader (classType);

	text += "\t}\n\n";

	return text;
}

std::string ComponentGenerator::GenerateSaver (const ClassType* classType)
{
	std::string text;

	text = "public:\n\tvoid Save (TiXmlElement* xmlElem) const\n\t{\n";

	text += GenerateAttributesSaver (classType);

	text += "\t}\n\n";

	return text;
}

std::string ComponentGenerator::GenerateWidget (const ClassType* classType)
{
	std::string text;

	text = "public:\n\tvoid Show ()\n\t{\n"
			"\t\tImGui::Separator ();\n"
			"\t\tif (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {\n";

	text += GenerateAttributesWidget (classType);

	text += "\t\t\tImGui::PushID (this);\n"
			"\t\t\tif (ImGui::Button (\"Detach Component\", ImVec2 (ImGui::GetWindowWidth (), 18))) {\n"
			"\t\t\t\t_parent->DetachComponent (this);\n"
			"\t\t\t}\n"
			"\t\t\tImGui::PopID ();\n";

	text += "\t\t}\n"
			"\t}\n\n";

	return text;
}

std::string ComponentGenerator::GenerateAttributesLoader (const ClassType* classType)
{
	std::string text;

	if (classType->IsActive == false) {
		return text;
	}

	for (auto parentType : classType->Parents) {
		text += GenerateAttributesLoader (parentType);
	}

	text += "\t\tfor (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {\n";
	for (auto attribute : classType->Attributes) {
		text += "\t\t\tif (std::string (attribute->Name ()) == \"" + trim (attribute.Name) + "\") {\n"
				"\t\t\t\t" + attribute.Name + " = ComponentAttributeLoader::GetValue<" + attribute.TypeName + "> (attribute->Value ());\n"
				"\t\t\t}\n";
	}
	text += "\t\t}\n";

	text += "\t\tfor (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {\n";
	for (auto attribute : classType->Attributes) {

		text += "\t\t\tif (std::string (content->Value ()) == \"" + trim(attribute.Name) + "\") {\n"
				"\t\t\t\t" + attribute.Name + " = ComponentAttributeLoader::Load<" + attribute.TypeName + "> (content);\n"
				"\t\t\t}\n";
	}
	text += "\t\t}\n";

	return text;
}

std::string ComponentGenerator::GenerateAttributesSaver (const ClassType* classType)
{
	std::string text;

	if (classType->IsActive == false) {
		return text;
	}

	for (auto parentType : classType->Parents) {
		text += GenerateAttributesSaver (parentType);
	}

	for (auto attribute : classType->Attributes) {
		text += "\t\tComponentAttributeSaver::Save<" + attribute.TypeName + "> (xmlElem, " + attribute.Name + ", \"" + trim (attribute.Name) + "\");\n";
	}

	return text;
}

std::string ComponentGenerator::GenerateAttributesWidget (const ClassType* classType)
{
	std::string text;

	if (classType->IsActive == false) {
		return text;
	}

	for (auto parentType : classType->Parents) {
		text += GenerateAttributesWidget (parentType);
	}

	for (auto attribute : classType->Attributes) {
		text += "\t\t\tImGui::PushID (&" + attribute.Name + ");\n"
				"\t\t\tif (ComponentAttributeWidget::Show<" + attribute.TypeName + "> (" + attribute.Name + ", \"" + trim (attribute.Name) + "\")) {\n"
				"\t\t\t\tSet" + capitalise (trim (attribute.Name)) + " (" + attribute.Name + ");\n"
				"\t\t\t}\n"
				"\t\t\tImGui::PopID ();\n";
	}

	return text;
}
