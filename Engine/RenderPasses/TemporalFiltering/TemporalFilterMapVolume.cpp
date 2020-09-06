#include "TemporalFilterMapVolume.h"

TemporalFilterMapVolume::TemporalFilterMapVolume (const Resource<Framebuffer>& framebuffer) :
	FramebufferRenderVolume (framebuffer),
	_viewProjectionMatrix (1.0f),
	_current (false)
{
	SetCurrent (_current);
}

void TemporalFilterMapVolume::SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix)
{
	_viewProjectionMatrix = viewProjectionMatrix;
}

void TemporalFilterMapVolume::SetCurrent (bool current)
{
	_current = current;

	/*
	 * Update attributes
	*/

	_attributes.clear ();

	PipelineAttribute temporalFilterMap;

	temporalFilterMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	temporalFilterMap.name = _current == true ? "postProcessMap" : "temporalFilterMap";

	temporalFilterMap.value.x = _framebufferView->GetTextureView (0)->GetGPUIndex ();

	_attributes.push_back (temporalFilterMap);
}

const glm::mat4& TemporalFilterMapVolume::GetViewProjectionMatrix () const
{
	return _viewProjectionMatrix;
}

const std::vector<PipelineAttribute>& TemporalFilterMapVolume::GetCustomAttributes () const
{
	return _attributes;
}
