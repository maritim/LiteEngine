#ifndef ANIMATIONGAMEOBJECT
#define ANIMATIONGAMEOBJECT

#include "GameObject.h"

class ENGINE_API AnimationGameObject : public GameObject
{
public:
	AnimationGameObject ();
	~AnimationGameObject ();

	void AttachMesh (const Resource<Model>& mesh);

	void SetAnimationClip (const std::string& animName);
	void Blend (const std::string& nextAnimName, float duration);
};

#endif