#include "RSMVolume.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

RSMVolume::RSMVolume (const Resource<Framebuffer>& framebuffer) :
	FramebufferRenderVolume (framebuffer),
	_camera (nullptr)
{
	/*
	 * Update attributes
	*/

	PipelineAttribute lightSpaceMatrix;
	PipelineAttribute lightViewMatrix;
	PipelineAttribute lightProjectionMatrix;
	PipelineAttribute shadowBias;

	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	lightViewMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	lightProjectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	lightSpaceMatrix.name = "lightSpaceMatrix";
	lightViewMatrix.name = "lightViewMatrix";
	lightProjectionMatrix.name = "lightProjectionMatrix";
	shadowBias.name = "shadowBias";

	_attributes.push_back (lightSpaceMatrix);
	_attributes.push_back (lightViewMatrix);
	_attributes.push_back (lightProjectionMatrix);
	_attributes.push_back (shadowBias);
}

RSMVolume::~RSMVolume ()
{
	delete _camera;
}

void RSMVolume::SetLightCamera (Camera* camera)
{
	_camera = camera;

	/*
	 * Update attributes
	*/

	glm::mat4 lightProjection = _camera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (_camera->GetRotation ()), _camera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	_attributes [4].matrix = screenMatrix * lightProjection * lightView;
	_attributes [5].matrix = lightView;
	_attributes [6].matrix = lightProjection;
}

void RSMVolume::SetShadowBias (float shadowBias)
{
	/*
	 * Update attributes
	*/

	_attributes [7].value.x = shadowBias;
}

Camera* RSMVolume::GetLightCamera ()
{
	return _camera;
}
