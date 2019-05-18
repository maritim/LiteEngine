#ifndef COLLIDER_H
#define COLLIDER_H

#include "Core/Intersections/GeometricPrimitive.h"

#include "Mesh/Model.h"
#include "SceneGraph/Transform.h"

class ENGINE_API Collider
{
protected:
	GeometricPrimitive* _primitive;

public:
	Collider ();
	virtual ~Collider ();

	void virtual Rebuild (Model* mesh, Transform* transform) = 0;

	GeometricPrimitive* GetGeometricPrimitive () const;
protected:
	void DestroyCurrentPrimitive ();
};

#endif