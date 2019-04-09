#include "Renderer.h"

Renderer::Renderer() :
	_stage (DEFERRED_STAGE),
	_priority (0),
	_transform (new Transform ())
{

}

Renderer::Renderer (Transform* transform) :
	_stage (DEFERRED_STAGE),
	_priority (0),
	_transform (transform)
{
	
}

Renderer::~Renderer()
{

}

void Renderer::SetStageType (Renderer::StageType stage)
{
	_stage = stage;
}

Renderer::StageType Renderer::GetStageType () const
{
	return _stage;
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

void Renderer::DrawGeometry ()
{
	/*
	 * Do nothing
	 *
	 * Intended to draw only the geometry without a material
	*/
}