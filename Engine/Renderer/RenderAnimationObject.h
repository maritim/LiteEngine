#ifndef RENDERANIMATIONOBJECT_H
#define RENDERANIMATIONOBJECT_H

#include "RenderObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Renderer/PipelineAttribute.h"

//TODO: Fix this
#include "Core/Resources/Resource.h"
#include "Mesh/AnimationModel.h"

class RenderAnimationObject : public RenderObject
{
protected:
	Resource<Model> _animationModel;
	std::string _currentAnimClipName;
	float _currAnimStartTime;

	std::string _previousAnimClipName;
	float _blendStartTime;
	float _globalBlendStartTime;
	float _blendDuration;

public:
	RenderAnimationObject ();

	void Draw ();

	//TODO: Fix this
	void SetAnimationModel (const Resource<Model>& animationModel);
	void SetAnimationClip (const std::string& animName);
	void Blend (const std::string& nextAnimName, float duration);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();

	void ProcessBoneTransform (AnimationModel* animModel, const glm::mat4& parentTransform,
		AnimationContainer* prevAnimContainer, AnimationContainer* currAnimContainer, BoneNode* boneNode,
		const glm::mat4& inverseGlobalMatrix, float prevAnimTime, float currAnimTime,
		float blendWeight, std::vector<glm::mat4>& boneTransform);

	void CalcInterpolatedScaling (glm::vec3& scaling, float animTime, AnimationNode* animNode);
	void CalcInterpolatedPosition (glm::vec3& position, float animTime, AnimationNode* animNode);
	void CalcInterpolatedRotation (glm::quat& rotation, float animTime, AnimationNode* animNode);
};

#endif