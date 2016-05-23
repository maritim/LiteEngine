#ifndef ANIMATIONSCONTAINER_H
#define ANIMATIONSCONTAINER_H

#include "Core/Interfaces/Object.h"

#include <map>
#include <string>

#include "AnimationNode.h"

class AnimationContainer : public Object
{
protected:
	std::string _name;
	float _duration;
	float _ticksPerSecond;
	std::map<std::string, AnimationNode*> _animationNodes;

public:
	AnimationContainer ();
	~AnimationContainer ();

	AnimationNode* GetAnimationNode (const std::string& boneName) const;
	std::string GetName () const;
	float GetDuration () const;
	float GetTicksPerSecond () const;

	void AddAnimationNode (const std::string& boneName, AnimationNode* animNode);
	void SetName (const std::string& name);
	void SetDuration (float duration);
	void SetTicksPerSecond (float ticksPerSecond);
};

#endif