#include "AnimationGameObject.h"

#include "AnimationModel3DRenderer.h"

AnimationGameObject::AnimationGameObject () :
	GameObject ()
{
	delete _renderer;
	_renderer = new AnimationModel3DRenderer (_transform);
	_renderer->SetPriority (1);
}

AnimationGameObject::~AnimationGameObject ()
{
	
}