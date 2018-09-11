#include "BulletCollider.h"

BulletCollider::BulletCollider () :
	_collisionShape (nullptr),
	_offset (0.0f)
{

}

BulletCollider::~BulletCollider ()
{
	DestroyCollisionShape ();
}

void BulletCollider::SetOffset (const glm::vec3& offset)
{
	_offset = offset;
}

btCollisionShape* BulletCollider::GetCollisionShape () const
{
	return _collisionShape;
}

glm::vec3 BulletCollider::GetOffset () const
{
	return _offset;
}

void BulletCollider::DestroyCollisionShape ()
{
	if (_collisionShape != nullptr) {
		return;
	}

	delete _collisionShape;
}