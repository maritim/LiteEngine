#include "NormalMapGameObject.h"

#include "NormalMapModel3DRenderer.h"

#include "SceneNodes/SceneLayer.h"

NormalMapGameObject::NormalMapGameObject () :
	GameObject ()
{
	delete _renderer;
	_renderer = new NormalMapModel3DRenderer (_transform);
	_renderer->SetPriority (1);
	_sceneLayers = (int) SceneLayer::STATIC;
}

NormalMapGameObject::~NormalMapGameObject ()
{

}