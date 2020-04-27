#ifndef SPHERECOLLIDERCOMPONENT_H
#define SPHERECOLLIDERCOMPONENT_H

#include "ColliderComponent.h"

class ENGINE_API SphereColliderComponent : public ColliderComponent
{
	DECLARE_COMPONENT(SphereColliderComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _radius;

public:
	SphereColliderComponent ();

	void Awake ();

	void SetModel (const Resource<Model>& model);
	void SetRadius (float radius);

	float GetRadius () const;
protected:
	float GetRadius (const Resource<Model>& model);
};

#endif