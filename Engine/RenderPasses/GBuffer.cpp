#include "GBuffer.h"

GBuffer::GBuffer (const Resource<Framebuffer>& framebuffer) :
	FramebufferRenderVolume (framebuffer),
	_projectionMatrix (1.0f),
	_frustumJitter (0.0f)
{
	/*
	 * Create attributes
	*/

	PipelineAttribute screenSize;

	screenSize.type = PipelineAttribute::AttrType::ATTR_2F;

	screenSize.name = "screenSize";

	auto resolution = _framebuffer->GetTexture (0)->GetSize ();

	screenSize.value = glm::vec3 (resolution.width, resolution.height, 0.0f);

	_attributes.push_back (screenSize);
}

void GBuffer::SetProjectionMatrix (const glm::mat4& projectionMatrix)
{
	_projectionMatrix = projectionMatrix;
}

void GBuffer::SetFrustumJitter (const glm::vec2& frustumJitter)
{
	_frustumJitter = frustumJitter;
}

const glm::mat4& GBuffer::GetProjectionMatrix () const
{
	return _projectionMatrix;
}

const glm::vec2& GBuffer::GetFrustumJitter () const
{
	return _frustumJitter;
}
