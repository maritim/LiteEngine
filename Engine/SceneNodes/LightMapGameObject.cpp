#include "LightMapGameObject.h"

#include "LightMapModel3DRenderer.h"

#include "SceneNodes/SceneLayer.h"

LightMapGameObject::LightMapGameObject () :
	GameObject ()
{
	delete _renderer;
	_renderer = new LightMapModel3DRenderer (_transform);
	_renderer->SetPriority (1);
	_sceneLayers = (int) SceneLayer::STATIC | (int) SceneLayer::LIGHT_MAP;
}

LightMapGameObject::~LightMapGameObject ()
{

}