#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Core/Interfaces/Object.h"

#include "SceneGraph/Transform.h"

class Rigidbody : public Object
{
protected:
	Transform* _transform;
	bool _useGravity;

public:
	Rigidbody (Transform* _transform);

	bool GetGravityUse () const;

	void SetGravityUse (bool useGravity);

	void Update ();
};

#endif