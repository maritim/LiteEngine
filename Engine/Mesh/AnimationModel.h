#ifndef ANIMATIONMODEL_H
#define ANIMATIONMODEL_H

#include "Model.h"

#include <map>

#include "AnimationsController.h"

#include "VertexBoneInfo.h"
#include "BoneTree.h"
#include "BoneInfo.h"

class ENGINE_API AnimationModel : public Model
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
	VertexBoneInfo* GetVertexBoneInfo (std::size_t index) const;

	void SetAnimationsController (AnimationsController* animController);
	AnimationsController* GetAnimationsController () const;

	BoneTree* GetBoneTree () const;
	void SetBoneTree (BoneTree* boneTree);

	void AddBone (BoneInfo* bone);
	BoneInfo* GetBone (const std::string& name) const;
	BoneInfo* GetBone (std::size_t index) const;

	~AnimationModel ();
};

#endif