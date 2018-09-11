#ifndef COLLIDERLOADER_H
#define COLLIDERLOADER_H

#include "ResourceLoader.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Systems/Collision/BulletCollider.h"

class ColliderLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
protected:
	BulletCollider* CreateCollider (TiXmlElement* root);

	void ProcessOffset (TiXmlElement* xmlElem, BulletCollider* collider);

	BulletCollider* CreateBoxCollider (TiXmlElement* root);
	BulletCollider* CreateSphereCollider (TiXmlElement* root);
	BulletCollider* CreateCylinderCollider (TiXmlElement* root);
	BulletCollider* CreateCapsuleCollider (TiXmlElement* root);
};

#endif