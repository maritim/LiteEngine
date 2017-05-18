#include "AnimationGameObject.h"

#include "AnimationModel3DRenderer.h"

#include "SceneNodes/SceneLayer.h"

AnimationGameObject::AnimationGameObject () :
	GameObject ()
{
	delete _renderer;
	_renderer = new AnimationModel3DRenderer (_transform);
	_renderer->SetPriority (1);
	_sceneLayers = (int) SceneLayer::ANIMATION;
}

AnimationGameObject::~AnimationGameObject ()
{
	
}