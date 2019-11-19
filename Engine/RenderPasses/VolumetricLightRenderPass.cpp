#include "VolumetricLightRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Managers/ShaderManager.h"

VolumetricLightRenderPass::VolumetricLightRenderPass () :
	_stencilShaderName ("STENCIL_VOLUMETRIC_LIGHT")
{

}

VolumetricLightRenderPass::~VolumetricLightRenderPass ()
{

}

void VolumetricLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for stencil volumetric light render pass
	*/

	ShaderManager::Instance ()->AddShader (_stencilShaderName,
		"Assets/Shaders/deferredStencilVolLightVertex.glsl",
		"Assets/Shaders/deferredStencilVolLightFragment.glsl");
}

RenderVolumeCollection* VolumetricLightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	StartPointLightPass (rvc);

	/*
	 * Draw volumetric lights
	*/

	PointLightPass (renderScene, camera, settings, rvc);

	/*
	 * End directional light pass
	*/

	EndPointLightPass ();

	return rvc;
}

bool VolumetricLightRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute directional volumetric light render sub pass
	*/

	return true;
}

void VolumetricLightRenderPass::StartPointLightPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto resultFrameBufferVolume = rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");

	resultFrameBufferVolume->BindForWriting ();
}

void VolumetricLightRenderPass::PointLightPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Draw point light for stencil pass
	*/

	PointLightStencilPass (renderScene, camera, settings, renderLightObject, rvc);

	/*
	 * Draw volumetric point light
	*/

	PointLightDrawPass (renderScene, camera, settings, renderLightObject, rvc);
}

void VolumetricLightRenderPass::EndPointLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();

	GL::Disable (GL_DEPTH_CLAMP);

	GL::Disable (GL_STENCIL_TEST);
}

void VolumetricLightRenderPass::PointLightStencilPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Lock stencil volumetric light shader
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_stencilShaderName));

	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * No rendering target
	*/

	GL::DrawBuffer (GL_NONE);

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	 * Stencil pass technique. For more details please look at the link bellow.
	 * http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html
	 *
	 * It uses stencil test to identify the fragments that are in light.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 0, 0);

	GL::StencilOpSeparate (GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	GL::StencilOpSeparate (GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	/*
	 * Both faces need to be drawn.
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Ignore near and far planes
	*/

	GL::Enable (GL_DEPTH_CLAMP);

	/*
	 * Send camera to pipeline
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	/*
	 * Volumetric light draw.
	*/

	renderLightObject->Draw ();

	/*
	 * Unlock shader
	*/

	Pipeline::UnlockShader ();
}

void VolumetricLightRenderPass::PointLightDrawPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

	/*
	 * Lock shader for volumetric point light
	*/

	LockShader (renderLightObject);

	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto resultFrameBufferVolume = rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");

	resultFrameBufferVolume->BindForWriting ();

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	 * Pass the fragment only if it is like that
	 *
	 * 				(buffer frag) > (value '1')
	 *
	 * Don't override the sky, which have value '0'.
	 * Override only what fragments that are in light.
	 *
	 * After drawing, Stencil Buffer is refilled with 1's.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_LESS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	 * Blend between point lights with same weight.
	*/

	GL::Enable (GL_BLEND);
	GL::BlendEquation (GL_FUNC_ADD);
	GL::BlendFunc (GL_ONE, GL_ONE);

	/*
	 * Process fragments single time. Front Face randomly chosen.
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_FRONT);

	/*
	 * Ignore near and far planes
	*/

	GL::Enable (GL_DEPTH_CLAMP);

	/*
	 * Send camera to pipeline
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	/*
	 * Send custom attributes
	*/

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (camera, renderLightObject, rvc));

	/*
	 * Send custom attributes
	*/

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (renderLightObject));

	/*
	 * Draw the volumetric light.
	*/

	renderLightObject->DrawGeometry ();
}

std::vector<PipelineAttribute> VolumetricLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderLightObject* renderLightObject, RenderVolumeCollection* rvc) const
{
	std::vector<PipelineAttribute> attributes;

	for (RenderVolumeI* renderVolume : *rvc) {
		std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	/*
	 * Add render light object
	*/

	PipelineAttribute lightPosition;
	PipelineAttribute lightDirection;
	PipelineAttribute lightColor;
	PipelineAttribute lightIntensity;

	lightPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	lightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	lightIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lightPosition.name = "lightPosition";
	lightDirection.name = "lightDirection";
	lightColor.name = "lightColor";
	lightIntensity.name = "lightIntensity";

	glm::mat4 viewMatrix = glm::mat4_cast (camera->GetRotation ());
	viewMatrix =  glm::translate (viewMatrix, camera->GetPosition () * -1.0f);

	glm::vec3 lightPos = renderLightObject->GetTransform ()->GetPosition ();
	lightPos = viewMatrix * glm::vec4 (lightPos, 1.0f);

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);
	lightDir = glm::normalize (glm::vec3 (viewMatrix * glm::vec4 (lightDir, 0.0f)));

	lightPosition.value = lightPos;
	lightDirection.value = lightDir;
	lightColor.value = renderLightObject->GetLightColor ().ToVector3 ();
	lightIntensity.value.x = renderLightObject->GetLightIntensity ();

	attributes.push_back (lightPosition);
	attributes.push_back (lightDirection);
	attributes.push_back (lightColor);
	attributes.push_back (lightIntensity);

	return attributes;
}
