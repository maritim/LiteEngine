#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "ColliderComponent.h"

class ENGINE_API BoxColliderComponent : public ColliderComponent
{
	DECLARE_COMPONENT(BoxColliderComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	glm::vec3 _extents;

public:
	void Awake ();

	void SetModel (const Resource<Model>& model);
	virtual void SetExtents (const glm::vec3& extents);

	const glm::vec3& GetExtents () const;
protected:
	glm::vec3 GetExtents (const Resource<Model>& model);
};

#endif