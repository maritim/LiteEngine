#include "Component.h"

#include "ComponentManager.h"

Component::Component ()
{

}

Component::~Component ()
{

}

void Component::Awake ()
{

}

void Component::Start ()
{

}

void Component::Update ()
{

}

void Component::LateUpdate ()
{

}

void Component::SetActive (bool isActive)
{

}

void Component::OnAttachedToScene ()
{

}

void Component::OnDetachedFromScene ()
{

}

void Component::SetParent (SceneObject* parent)
{
	_parent = parent;
}
