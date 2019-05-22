#include "AnimationsController.h"

AnimationsController::~AnimationsController ()
{
	for (auto animContainer : _animations) {
		delete animContainer.second;
	}
}

AnimationContainer* AnimationsController::GetAnimationContainer (const std::string& animationName)
{
	auto animContainer = _animations.find (animationName);

	if (animContainer == _animations.end ()) {
		return nullptr;
	}

	return animContainer->second;
}

void AnimationsController::AddAnimationContainer (AnimationContainer* animContainer)
{
	std::string animContainerName = animContainer->GetName ();

	_animations [animContainerName] = animContainer;
}

AnimationsContainerIterator AnimationsController::begin ()
{
	AnimationsContainerIterator acIt (_animations.begin ());

	return acIt;
}

AnimationsContainerIterator AnimationsController::end ()
{
	AnimationsContainerIterator acIt (_animations.end ());

	return acIt;
}

/*
 * Animations container iterator
*/

AnimationsContainerIterator& AnimationsContainerIterator::operator++ ()
{
	_it++;

	return *this;
}

bool AnimationsContainerIterator::operator != (const AnimationsContainerIterator& other)
{
	return _it != other._it;
}

AnimationContainer* AnimationsContainerIterator::operator* ()
{
	return _it->second;
}

AnimationsContainerIterator::AnimationsContainerIterator (std::map<std::string, AnimationContainer*>::iterator it) :
	_it (it)
{

}