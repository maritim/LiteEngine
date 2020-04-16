#ifndef CAPSULECOLLIDERCOMPONENT_H
#define CAPSULECOLLIDERCOMPONENT_H

#include "SphereColliderComponent.h"

class ENGINE_API CapsuleColliderComponent : public SphereColliderComponent
{
	DECLARE_COMPONENT(CapsuleColliderComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _height;

public:
	void Awake ();

	void SetModel (const Resource<Model>& model);
	void SetRadius (float radius);
	void SetHeight (float height);

	float GetHeight () const;
protected:
	void SetSize (float radius, float height);

	glm::vec2 GetSize (const Resource<Model>& model);
};

#endif