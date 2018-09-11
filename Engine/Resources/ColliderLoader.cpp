#include "ColliderLoader.h"

#include "Systems/Collision/BoxCollider.h"
#include "Systems/Collision/SphereCollider.h"
#include "Systems/Collision/CylinderCollider.h"
#include "Systems/Collision/CapsuleCollider.h"
#include "Systems/Collision/MeshCollider.h"

#include "Utils/Extensions/StringExtend.h"

Object* ColliderLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		return nullptr;
	}

	TiXmlElement* root = doc.FirstChildElement ("Collider");

	if (root == nullptr) {
		return nullptr;
	}

	BulletCollider* collider = CreateCollider (root);

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Offset") {
			ProcessOffset (content, collider);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return collider;
}

BulletCollider* ColliderLoader::CreateCollider (TiXmlElement* root)
{
	std::string shape = root->Attribute ("shape");

	BulletCollider* collider = nullptr;

	if (shape == "Box") {
		collider = CreateBoxCollider (root);
	}
	else if (shape == "Sphere") {
		collider = CreateSphereCollider (root);
	}
	else if (shape == "Cylinder") {
		collider = CreateCylinderCollider (root);
	}
	else if (shape == "Capsule") {
		collider = CreateCapsuleCollider (root);
	}
	else if (shape == "Mesh") {
		collider = new MeshCollider ();
	}

	return collider;
}

void ColliderLoader::ProcessOffset (TiXmlElement* xmlElem, BulletCollider* collider)
{
	glm::vec3 vec;

	const char* x = xmlElem->Attribute ("x");
	const char* y = xmlElem->Attribute ("y");
	const char* z = xmlElem->Attribute ("z");

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

BulletCollider* ColliderLoader::CreateBoxCollider (TiXmlElement* root)
{
	std::string isGenerated = root->Attribute ("isGenerated");

	BoxCollider* collider = nullptr;

	if (Extensions::StringExtend::ToBool (isGenerated) == true) {
		collider = new BoxCollider ();
	}

	if (Extensions::StringExtend::ToBool (isGenerated) == false) {
		std::string x = root->Attribute ("x");
		std::string y = root->Attribute ("y");
		std::string z = root->Attribute ("z");

		collider = new BoxCollider (glm::vec3 (std::stof (x), std::stof (y), std::stof (z)));
	}

	return collider;
}

BulletCollider* ColliderLoader::CreateSphereCollider (TiXmlElement* root)
{
	std::string isGenerated = root->Attribute ("isGenerated");

	SphereCollider* collider = nullptr;

	if (Extensions::StringExtend::ToBool (isGenerated) == true) {
		collider = new SphereCollider ();
	}

	if (Extensions::StringExtend::ToBool (isGenerated) == false) {
		std::string radius = root->Attribute ("radius");

		collider = new SphereCollider (std::stof (radius));
	}

	return collider;
}

BulletCollider* ColliderLoader::CreateCylinderCollider (TiXmlElement* root)
{
	std::string isGenerated = root->Attribute ("isGenerated");

	CylinderCollider* collider = nullptr;

	if (Extensions::StringExtend::ToBool (isGenerated) == true) {
		collider = new CylinderCollider ();
	}

	if (Extensions::StringExtend::ToBool (isGenerated) == false) {
		std::string radius = root->Attribute ("radius");
		std::string height = root->Attribute ("height");

		collider = new CylinderCollider (std::stof (radius), std::stof (height));
	}

	return collider;
}

BulletCollider* ColliderLoader::CreateCapsuleCollider (TiXmlElement* root)
{
	std::string isGenerated = root->Attribute ("isGenerated");

	CapsuleCollider* collider = nullptr;

	if (Extensions::StringExtend::ToBool (isGenerated) == true) {
		collider = new CapsuleCollider ();
	}

	if (Extensions::StringExtend::ToBool (isGenerated) == false) {
		std::string radius = root->Attribute ("radius");
		std::string height = root->Attribute ("height");

		collider = new CapsuleCollider (std::stof (radius), std::stof (height));
	}

	return collider;
}