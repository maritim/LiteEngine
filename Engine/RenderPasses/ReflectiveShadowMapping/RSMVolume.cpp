#include "RSMVolume.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

RSMVolume::RSMVolume () :
	_shadowMapBuffer (new RSMBuffer ()),
	_size (0),
	_camera (nullptr),
	_shadowBias (0.0f)
{

}

RSMVolume::~RSMVolume ()
{
	delete _shadowMapBuffer;
	delete _camera;
}

bool RSMVolume::Init (const glm::ivec2& size)
{
	/*
	 * Initialize resolution
	*/

	_size = size;

	/*
	 * Initialize
	*/

	return _shadowMapBuffer->Init (_size);
}

void RSMVolume::SetLightCamera (Camera* camera)
{
	_camera = camera;
}

void RSMVolume::SetShadowBias (float shadowBias)
{
	_shadowBias = shadowBias;
}

Camera* RSMVolume::GetLightCamera ()
{
	return _camera;
}

void RSMVolume::BindForWriting ()
{
	/*
	 * Change resolution on viewport as shadow map size
	*/

	glm::ivec2 size = _shadowMapBuffer->GetSize ();
	GL::Viewport (0, 0, size.x, size.y);

	/*
	 * Bind reflective shadow map buffer
	*/

	_shadowMapBuffer->BindForWriting ();
}

void RSMVolume::EndDrawing ()
{
	Pipeline::UnlockShader ();

	GL::DrawBuffer(GL_NONE);
	GL::ReadBuffer(GL_NONE);

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSMVolume::BindForReading ()
{
	/*
	 * Bind every depth buffer for reading
	 * TODO: Extend this to cascaded reflective shadow mapping
	*/

	_shadowMapBuffer->BindForReading ();
}

RSMBuffer* RSMVolume::GetShadowMapBuffer () const
{
	return _shadowMapBuffer;
}

glm::ivec2 RSMVolume::GetSize () const
{
	return _size;
}

void RSMVolume::Clear ()
{
	/*
	 * Clear shadow map buffer
	*/

	_shadowMapBuffer->Clear ();
}

std::vector<PipelineAttribute> RSMVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute reflectiveShadowMapDepth;
	PipelineAttribute reflectiveShadowMapPos;
	PipelineAttribute reflectiveShadowMapNormal;
	PipelineAttribute reflectiveShadowMapFlux;
	PipelineAttribute lightSpaceMatrix;
	PipelineAttribute lightViewMatrix;
	PipelineAttribute lightProjectionMatrix;
	PipelineAttribute shadowBias;

	reflectiveShadowMapDepth.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	reflectiveShadowMapPos.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	reflectiveShadowMapNormal.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	reflectiveShadowMapFlux.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	lightViewMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	lightProjectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	reflectiveShadowMapDepth.name = "shadowMap";
	reflectiveShadowMapPos.name = "rsmPositionMap";
	reflectiveShadowMapNormal.name = "rsmNormalMap";
	reflectiveShadowMapFlux.name = "rsmFluxMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";
	lightViewMatrix.name = "lightViewMatrix";
	lightProjectionMatrix.name = "lightProjectionMatrix";
	shadowBias.name = "shadowBias";

	reflectiveShadowMapDepth.value.x = _shadowMapBuffer->GetDepthBuffer ();
	reflectiveShadowMapPos.value.x = _shadowMapBuffer->GetColorBuffer (0);
	reflectiveShadowMapNormal.value.x = _shadowMapBuffer->GetColorBuffer (1);
	reflectiveShadowMapFlux.value.x = _shadowMapBuffer->GetColorBuffer (2);

	glm::mat4 lightProjection = _camera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (_camera->GetRotation ()), _camera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	lightSpaceMatrix.matrix = screenMatrix * lightProjection * lightView;
	lightViewMatrix.matrix = lightView;
	lightProjectionMatrix.matrix = lightProjection;
	shadowBias.value.x = _shadowBias;

	attributes.push_back (reflectiveShadowMapDepth);
	attributes.push_back (reflectiveShadowMapPos);
	attributes.push_back (reflectiveShadowMapNormal);
	attributes.push_back (reflectiveShadowMapFlux);
	attributes.push_back (lightSpaceMatrix);
	attributes.push_back (lightViewMatrix);
	attributes.push_back (lightProjectionMatrix);
	attributes.push_back (shadowBias);

	return attributes;
}
