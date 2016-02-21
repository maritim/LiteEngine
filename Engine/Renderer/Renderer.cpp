#include "Renderer.h"

Renderer::Renderer() :
	_priority (0),
	_transform (new Transform ())
{
}

Renderer::Renderer (Transform* transform) :
	_priority (0),
	_transform (transform)
{
	
}

Renderer::~Renderer()
{

}

void Renderer::SetPriority (std::size_t priority)
{
	_priority = priority;
}

std::size_t Renderer::GetPriority () const
{
	return _priority;
}

Transform* Renderer::GetTransform () const
{
	return _transform;
}

void Renderer::Draw ()
{
	// Do nothing
	// It is supposed to inherit the class and implement this function
}