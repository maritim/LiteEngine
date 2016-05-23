#ifndef ANIMATIONMODEL_H
#define ANIMATIONMODEL_H

#include "Model.h"

#include <map>

#include "AnimationsController.h"

#include "VertexBoneInfo.h"
#include "BoneTree.h"
#include "BoneInfo.h"

class AnimationModel : public Model
{
protected:
	std::map<std::string, std::size_t> _boneMapping;
	std::vector<BoneInfo*> _bones;
	std::vector<VertexBoneInfo*> _bonesInfo;
	AnimationsController* _animController;
	BoneTree* _boneTree;

public:
	AnimationModel ();
	AnimationModel (const AnimationModel& other);

	std::size_t GetBoneCount () const;

	VertexBoneInfo* GetVertexBoneInfo (std::size_t index);

	void SetAnimationsController (AnimationsController* animController);
	AnimationsController* GetAnimationsController ();

	BoneTree* GetBoneTree ();
	void SetBoneTree (BoneTree* boneTree);

	void AddBone (BoneInfo* bone);
	BoneInfo* GetBone (const std::string& name);
	BoneInfo* GetBone (std::size_t index);

	~AnimationModel ();
};

#endif