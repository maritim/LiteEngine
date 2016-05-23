#ifndef ANIMATIONSCONTROLLER_H
#define ANIMATIONSCONTROLLER_H

#include "Core/Interfaces/Object.h"

#include <map>
#include <string>

#include "AnimationContainer.h"

class AnimationsController : public Object
{
protected:
	std::map<std::string, AnimationContainer*> _animations;

public:
	AnimationContainer* GetAnimationContainer (const std::string& animationName);
	void AddAnimationContainer (AnimationContainer* animContainer);
};

#endif