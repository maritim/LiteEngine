#include "TRSMIndirectDiffuseLightMapVolume.h"

TRSMIndirectDiffuseLightMapVolume::TRSMIndirectDiffuseLightMapVolume (const Resource<Framebuffer>& framebuffer) :
	FramebufferRenderVolume (framebuffer),
	_viewProjectionMatrix (1.0f),
	_current (true)
{
	SetCurrent (_current);
}

void TRSMIndirectDiffuseLightMapVolume::SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix)
{
	_viewProjectionMatrix = viewProjectionMatrix;
}

void TRSMIndirectDiffuseLightMapVolume::SetCurrent (bool current)
{
	_current = current;

	/*
	 * Update attributes
	*/

	_attributes.clear ();

	PipelineAttribute indirectDiffuseMap;

	indirectDiffuseMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectDiffuseMap.name = _current == true ? "indirectDiffuseMap" : "lastIndirectDiffuseMap";

	indirectDiffuseMap.value.x = _framebufferView->GetTextureView (0)->GetGPUIndex ();

	_attributes.push_back (indirectDiffuseMap);
}

const glm::mat4& TRSMIndirectDiffuseLightMapVolume::GetViewProjectionMatrix () const
{
	return _viewProjectionMatrix;
}
