#include "Rigidbody.h"

#include "Physics.h"

#include "Systems/Time/Time.h"

Rigidbody::Rigidbody (Transform* transform) :
	_transform (transform),
	_useGravity (true)
{

}

bool Rigidbody::GetGravityUse () const
{
	return _useGravity;
}

void Rigidbody::SetGravityUse (bool useGravity)
{
	_useGravity = useGravity;
}

void Rigidbody::Update ()
{
	if (!_useGravity) {
		return;
	}

	Vector3 currentPosition = _transform->GetPosition ();
	Vector3 nextPosition = currentPosition + 
		Physics::Instance ().GetGravityVector () * Time::GetDeltaTime ();

	_transform->SetPosition (nextPosition);
}