#include "RenderAnimationObject.h"

#include "Renderer/Pipeline.h"

#include "Mesh/VertexBoneInfo.h"
#include "Mesh/AnimationsController.h"
#include "Mesh/BoneTree.h"

#include "Systems/Time/Time.h"

#include "Utils/Conversions/Matrices.h"

RenderAnimationObject::RenderAnimationObject () :
	_animationModel (nullptr),
	_blendStartTime (0.0),
	_blendDuration (0.0f)
{

}

void RenderAnimationObject::Draw ()
{
	Pipeline::SetObjectTransform (_transform);

	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes ());

	_modelView->Draw ();
}

void RenderAnimationObject::DrawGeometry ()
{
	Pipeline::SetObjectTransform (_transform);

	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes ());

	_modelView->DrawGeometry ();
}

void RenderAnimationObject::SetAnimationModel (const Resource<Model>& animationModel)
{
	_animationModel = animationModel;
}

void RenderAnimationObject::SetAnimationClip (const std::string& animName)
{
	_previousAnimClipName = std::string ();
	_currentAnimClipName = animName;
	_currAnimStartTime = Time::GetTime ();
}

void RenderAnimationObject::Blend (const std::string& nextAnimName, float duration)
{
	_previousAnimClipName = _currentAnimClipName;
	_currentAnimClipName = nextAnimName;
	_blendDuration = duration;
	_globalBlendStartTime = Time::GetTime ();
	_blendStartTime = Time::GetTime () - _currAnimStartTime;
}

std::vector<PipelineAttribute> RenderAnimationObject::GetCustomAttributes ()
{
	AnimationModel* animModel = dynamic_cast<AnimationModel*> (&*_animationModel);

	// Calculate attribute
	std::vector<glm::mat4> boneTransform (animModel->GetBoneCount (), glm::mat4 (1.0f));

	AnimationsController* animController = animModel->GetAnimationsController ();
	AnimationContainer* prevAnimContainer = animController->GetAnimationContainer (_previousAnimClipName);
	AnimationContainer* animContainer = animController->GetAnimationContainer (_currentAnimClipName);

	if (animContainer != nullptr) {
		BoneTree* boneTree = animModel->GetBoneTree ();
		glm::mat4 rootMatrix (1.0f);

		glm::mat4 globalInverse = boneTree->GetRoot ()->GetTransform ();
		globalInverse = glm::inverse (globalInverse);

		float animationTime = Time::GetTime () - _currAnimStartTime;
		float ticksPerSecond = animContainer->GetTicksPerSecond () != 0 ? animContainer->GetTicksPerSecond () : 25.0f;
		float timeInTicks = animationTime * ticksPerSecond;
		animationTime = std::fmod (timeInTicks, animContainer->GetDuration ());

		if (prevAnimContainer != nullptr) {
			float blendElapsedTime = Time::GetTime () - _globalBlendStartTime;
			float blendWeight = blendElapsedTime / _blendDuration;

			float prevAnimationTime = _blendStartTime;
			float prevTicksPerSecond = prevAnimContainer->GetTicksPerSecond () != 0 ? prevAnimContainer->GetTicksPerSecond () : 25.0f;
			float prevTimeInTicks = prevAnimationTime * prevTicksPerSecond;
			prevAnimationTime = std::fmod (prevTimeInTicks, prevAnimContainer->GetDuration ());

			animationTime = 0.0f;

			ProcessBoneTransform (animModel, rootMatrix, prevAnimContainer, animContainer,
				boneTree->GetRoot (), globalInverse, prevAnimationTime, animationTime, blendWeight, boneTransform);

			if (blendElapsedTime > _blendDuration) {
				_previousAnimClipName = std::string ();
				_currAnimStartTime = Time::GetTime ();
			}
		}

		if (prevAnimContainer == nullptr) {
			ProcessBoneTransform (animModel, rootMatrix, nullptr, animContainer,
				boneTree->GetRoot (), globalInverse, 0.0f, animationTime, 0.0f, boneTransform);
		}
	}

	// Create attribute
	std::vector<PipelineAttribute> attributes;

	for (std::size_t i=0;i<boneTransform.size ();i++) {

		PipelineAttribute boneTransformAttr;

		boneTransformAttr.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

		boneTransformAttr.name = "boneTransforms[" + std::to_string (i) + "]";

		boneTransformAttr.matrix = boneTransform [i];

		attributes.push_back (boneTransformAttr);
	}

	return attributes;
}

void RenderAnimationObject::ProcessBoneTransform (AnimationModel* animModel, const glm::mat4& parentTransform, 
	AnimationContainer* prevAnimContainer, AnimationContainer* currAnimContainer, BoneNode* boneNode,
	const glm::mat4& inverseGlobalMatrix, float prevAnimTime, float currAnimTime,
	float blendWeight, std::vector<glm::mat4>& boneTransform)
{
	std::string nodeName = boneNode->GetName ();
	glm::mat4 nodeTransform = boneNode->GetTransform ();

	AnimationNode* prevAnimNode = prevAnimContainer != nullptr ? prevAnimContainer->GetAnimationNode (nodeName) : nullptr;
	AnimationNode* currAnimNode = currAnimContainer->GetAnimationNode (nodeName);

	if (currAnimNode != nullptr) {
		glm::vec3 scaling, prevScaling, currScaling;
		CalcInterpolatedScaling (currScaling, currAnimTime, currAnimNode);

		if (prevAnimContainer != nullptr) {
			CalcInterpolatedScaling (prevScaling, prevAnimTime, prevAnimNode);

			scaling = prevScaling * (1.0f - blendWeight) + currScaling * blendWeight;
		} else {
			scaling = currScaling;
		}

		glm::mat4 scalingM = glm::scale (glm::mat4 (1.0f), scaling);

		glm::quat rotationQ, prevRotationQ, currRotationQ;
		CalcInterpolatedRotation (currRotationQ, currAnimTime, currAnimNode);

		if (prevAnimContainer != nullptr) {
			CalcInterpolatedRotation(prevRotationQ, prevAnimTime, prevAnimNode);

			rotationQ = glm::slerp (prevRotationQ, currRotationQ, blendWeight);
		} else {
			rotationQ = currRotationQ;
		}

		glm::mat4 rotationM = glm::mat4_cast (rotationQ);

		glm::vec3 translation, prevTranslation, currTranslation;
		CalcInterpolatedPosition (currTranslation, currAnimTime, currAnimNode);

		if (prevAnimContainer != nullptr) {
			CalcInterpolatedPosition(prevTranslation, prevAnimTime, prevAnimNode);

			translation = prevTranslation * (1.0f - blendWeight) + currTranslation * blendWeight;
		} else {
			translation = currTranslation;
		}

		glm::mat4 translationM = glm::translate (glm::mat4 (1), translation);

		nodeTransform = translationM * rotationM * scalingM;
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	BoneInfo* boneInfo = animModel->GetBone (nodeName);

	if (boneInfo != nullptr) {
		boneTransform [boneInfo->GetID ()] = (inverseGlobalMatrix * globalTransformation) * boneInfo->GetTransformMatrix ();
	}

	for (std::size_t i=0;i<boneNode->GetChildrenCount ();i++) {
		ProcessBoneTransform (animModel, globalTransformation, prevAnimContainer, currAnimContainer,
			boneNode->GetChild (i), inverseGlobalMatrix, prevAnimTime, currAnimTime, blendWeight, boneTransform);
	}
}

void RenderAnimationObject::CalcInterpolatedRotation(glm::quat& rotationQ, float animationTime, AnimationNode* animNode)
{
	if (animNode->GetRotationKeysCount () == 1) {
		rotationQ = animNode->GetRotationKey (0).value;
		return ;
	}	

	std::size_t rotationIndex = 0;
	std::size_t nextRotationIndex = 0;

	while (rotationIndex < animNode->GetRotationKeysCount () - 1 &&
		animNode->GetRotationKey (rotationIndex + 1).time < animationTime) {
		++ rotationIndex;
	}

	nextRotationIndex = rotationIndex + 1;

	float deltaTime = animNode->GetRotationKey (nextRotationIndex).time - animNode->GetRotationKey (rotationIndex).time;
	float factor = (animationTime - animNode->GetRotationKey (rotationIndex).time) / deltaTime;

	const glm::quat& start = animNode->GetRotationKey (rotationIndex).value;
	const glm::quat& end = animNode->GetRotationKey (nextRotationIndex).value;

	rotationQ = glm::slerp(start, end, (float)factor);
	rotationQ = glm::normalize (rotationQ);
}

void RenderAnimationObject::CalcInterpolatedScaling (glm::vec3& scalingV, float animationTime, AnimationNode* animNode)
{
	if (animNode->GetScalingKeysCount () == 1) {
		scalingV = animNode->GetScalingKey (0).value;
		return ;
	}	

	std::size_t scalingIndex = 0;
	std::size_t nextScalingIndex = 0;

	while (scalingIndex < animNode->GetScalingKeysCount () - 1 &&
		animNode->GetScalingKey (scalingIndex + 1).time < animationTime) {
		++ scalingIndex;
	}

	nextScalingIndex = scalingIndex + 1;

	float deltaTime = animNode->GetScalingKey (nextScalingIndex).time - animNode->GetScalingKey (scalingIndex).time;
	float factor = (animationTime - animNode->GetScalingKey (scalingIndex).time) / deltaTime;

	const glm::vec3& start = animNode->GetScalingKey (scalingIndex).value;
	const glm::vec3& end = animNode->GetScalingKey (nextScalingIndex).value;

	glm::vec3 delta = end - start;

	scalingV = start + factor * delta;
} 

void RenderAnimationObject::CalcInterpolatedPosition (glm::vec3& positionV, float animationTime, AnimationNode* animNode)
{
	if (animNode->GetPositionKeysCount () == 1) {
		positionV = animNode->GetPositionKey (0).value;
		return ;
	}

	std::size_t positionIndex = 0;
	std::size_t nextPositionIndex = 0;

	while (positionIndex < animNode->GetPositionKeysCount () - 1 &&
		animNode->GetPositionKey (positionIndex + 1).time < animationTime) {
		++ positionIndex;
	}

	nextPositionIndex = positionIndex + 1;

	float deltaTime = animNode->GetPositionKey (nextPositionIndex).time - animNode->GetPositionKey (positionIndex).time;
	float factor = (animationTime - (float)animNode->GetPositionKey (positionIndex).time) / deltaTime;

	const glm::vec3& start = animNode->GetPositionKey (positionIndex).value;
	const glm::vec3& end = animNode->GetPositionKey (nextPositionIndex).value;
 
	glm::vec3 delta = end - start;

	positionV = start + factor * delta;
}
