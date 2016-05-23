#include "BoneInfo.h"

BoneInfo::BoneInfo () :
	_name ("")
{

}

void BoneInfo::SetName (const std::string& name)
{
	_name = name;
}

std::string BoneInfo::GetName () const
{
	return _name;
}

void BoneInfo::SetID (std::size_t id)
{
	_id = id;
}

std::size_t BoneInfo::GetID () const
{
	return _id;
}

void BoneInfo::SetTransformMatrix (aiMatrix4x4 transformMatrix)
{
	_transform = transformMatrix;
}

aiMatrix4x4 BoneInfo::GetTransformMatrix () const
{
	return _transform;
}