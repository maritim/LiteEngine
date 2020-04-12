#include "BulletCollider.h"

BulletCollider::BulletCollider () :
	_collisionShape (nullptr),
	_offset (0.0f),
	_isDirty (false)
{

}

BulletCollider::~BulletCollider ()
{
	DestroyCollisionShape ();
}

void BulletCollider::SetOffset (const glm::vec3& offset)
{
	_offset = offset;

	_isDirty = true;
}

void BulletCollider::SetMesh (const Resource<Model>& mesh)
{
	/*
	 * Rebuild collision shape
	*/

	Rebuild (mesh);

	_isDirty = true;
}

void BulletCollider::SetDirty (bool isDirty)
{
	_isDirty = isDirty;
}

btCollisionShape* BulletCollider::GetCollisionShape () const
{
	return _collisionShape;
}

glm::vec3 BulletCollider::GetOffset () const
{
	return _offset;
}

bool BulletCollider::IsDirty () const
{
	return _isDirty;
}

void BulletCollider::DestroyCollisionShape ()
{
	if (_collisionShape == nullptr) {
		return;
	}

	delete _collisionShape;
}