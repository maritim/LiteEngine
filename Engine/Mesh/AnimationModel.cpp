#include "AnimationModel.h"

#include "Core/Console/Console.h"

AnimationModel::AnimationModel () :
	Model (),
	_animController (nullptr),
	_boneTree (nullptr)
{

}

AnimationModel::AnimationModel (const AnimationModel& other) :
	Model (other),
	_bones (other._bones),
	_bonesInfo (other._bonesInfo),
	_animController (other._animController),
	_boneTree (other._boneTree)
{

}

AnimationModel::~AnimationModel ()
{
	for (std::size_t i = 0; i < _bones.size (); i++) {
		delete _bones [i];
	}

	for (std::size_t i = 0; i < _bonesInfo.size (); i++) {
		delete _bonesInfo [i];
	}

	delete _animController;
	delete _boneTree;
}

void AnimationModel::SetAnimationsController (AnimationsController* animController)
{
	if (_animController != nullptr) {
		delete _animController;
	}

	_animController = animController;
}

AnimationsController* AnimationModel::GetAnimationsController () const
{
	return _animController;
}

BoneTree* AnimationModel::GetBoneTree () const
{
	return _boneTree;
}

void AnimationModel::SetBoneTree (BoneTree* boneTree)
{
	if (_boneTree != nullptr) {
		delete _boneTree;
	}

	_boneTree = boneTree;
}

std::size_t AnimationModel::GetBoneCount () const
{
	return _boneMapping.size ();
}


VertexBoneInfo* AnimationModel::GetVertexBoneInfo (std::size_t index)
{
	while (index >= _bonesInfo.size ()) {
		_bonesInfo.push_back (new VertexBoneInfo ());
	}

	return _bonesInfo [index];
}

VertexBoneInfo* AnimationModel::GetVertexBoneInfo (std::size_t index) const
{
	return _bonesInfo [index];
}

void AnimationModel::AddBone (BoneInfo* boneInfo)
{
	boneInfo->SetID (_boneMapping.size ());

	_boneMapping [boneInfo->GetName ()] = boneInfo->GetID ();
	_bones.push_back (boneInfo);
}

BoneInfo* AnimationModel::GetBone (const std::string& name) const
{
	auto bone = _boneMapping.find (name);

	if (bone == _boneMapping.end ()) {
		return nullptr;
	}

	std::size_t index = bone->second;

	return _bones [index];
}

BoneInfo* AnimationModel::GetBone (std::size_t index) const
{
	return _bones [index];
}