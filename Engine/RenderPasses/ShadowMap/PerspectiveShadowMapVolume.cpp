#include "PerspectiveShadowMapVolume.h"

PerspectiveShadowMapVolume::PerspectiveShadowMapVolume (const Resource<Framebuffer>& framebuffer) :
	FramebufferRenderVolume (framebuffer),
	_lightCamera (nullptr)
{
	/*
	 * Create attributes
	*/

	PipelineAttribute lightSpaceMatrix;
	PipelineAttribute shadowBias;

	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	lightSpaceMatrix.name = "lightSpaceMatrix";
	shadowBias.name = "shadowBias";

	_attributes.push_back (lightSpaceMatrix);
	_attributes.push_back (shadowBias);
}

PerspectiveShadowMapVolume::~PerspectiveShadowMapVolume ()
{
	/*
	 * Delete light camera
	*/

	delete _lightCamera;
}

void PerspectiveShadowMapVolume::SetLightCamera (PerspectiveCamera* lightCamera)
{
	/*
	 * Keep light camera
	*/

	_lightCamera = lightCamera;

	/*
	 * Update attributes
	*/

	glm::mat4 lightProjection = _lightCamera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCamera->GetRotation ()), _lightCamera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	_attributes [0].matrix = screenMatrix * lightProjection * lightView;
}

void PerspectiveShadowMapVolume::SetShadowBias (float shadowBias)
{
	/*
	 * Update attributes
	*/

	_attributes [2].value.x = shadowBias;
}

PerspectiveCamera* PerspectiveShadowMapVolume::GetLightCamera ()
{
	return _lightCamera;
}
