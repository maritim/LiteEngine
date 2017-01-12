#include "Collider.h"

Collider::Collider () :
	_primitive (new GeometricPrimitive ())
{

}

Collider::~Collider ()
{
	DestroyCurrentPrimitive ();
}

GeometricPrimitive* Collider::GetGeometricPrimitive () const
{
	return _primitive;
}

void Collider::DestroyCurrentPrimitive ()
{
	if (_primitive == nullptr) {
		return;
	}

	delete _primitive;
}
