#include "AnimationsController.h"

#include "Core/Console/Console.h"

AnimationContainer* AnimationsController::GetAnimationContainer (const std::string& animationName)
{
	auto animContainer = _animations.find (animationName);

	if (animContainer == _animations.end ()) {
		Console::LogError ("There is no animation with name " + animationName + " in animation container!");

		return nullptr;
	}

	return animContainer->second;
}

void AnimationsController::AddAnimationContainer (AnimationContainer* animContainer)
{
	std::string animContainerName = animContainer->GetName ();

	_animations [animContainerName] = animContainer;
}