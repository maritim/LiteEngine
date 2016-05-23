#include "AnimationContainer.h"

AnimationContainer::AnimationContainer () :
	_name ("DEFAULT"),
	_duration (0),
	_ticksPerSecond (0)
{

}

AnimationContainer::~AnimationContainer ()
{
	for (auto animNode : _animationNodes) {
		delete animNode.second;
	}
}

AnimationNode* AnimationContainer::GetAnimationNode (const std::string& name) const
{
	auto animNode = _animationNodes.find (name);

	if (animNode == _animationNodes.end ()) {
		return nullptr;
	}

	return animNode->second;
}

std::string AnimationContainer::GetName () const
{
	return _name;
}

float AnimationContainer::GetDuration () const
{
	return _duration;
}

float AnimationContainer::GetTicksPerSecond () const
{
	return _ticksPerSecond;
}

void AnimationContainer::AddAnimationNode (const std::string& boneName, AnimationNode* animNode)
{
	_animationNodes [boneName] = animNode;
}

void AnimationContainer::SetName (const std::string& name)
{
	_name = name;
}

void AnimationContainer::SetDuration (float duration)
{
	_duration = duration;
}

void AnimationContainer::SetTicksPerSecond (float ticksPerSecond)
{
	_ticksPerSecond = ticksPerSecond;
}