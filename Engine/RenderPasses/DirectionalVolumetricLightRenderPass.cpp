#include "DirectionalVolumetricLightRenderPass.h"

#include "Renderer/Pipeline.h"

DirectionalVolumetricLightRenderPass::~DirectionalVolumetricLightRenderPass ()
{

}

RenderVolumeCollection* DirectionalVolumetricLightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	StartDirectionalLightPass (rvc);

	/*
	 * Draw volumetric lights
	*/

	DirectionalLightPass (renderScene, camera, settings, rvc);

	/*
	 * End directional light pass
	*/

	EndDirectionalLightPass ();

	return rvc;
}

bool DirectionalVolumetricLightRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute directional volumetric light render sub pass
	*/

	return true;
}

void DirectionalVolumetricLightRenderPass::StartDirectionalLightPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto resultFrameBuffer = rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");

	resultFrameBuffer->BindForWriting ();
}

void DirectionalVolumetricLightRenderPass::DirectionalLightPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Lock shader for volumetric light draw
	*/

	LockShader (renderLightObject);

	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_EQUAL, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	/*
	 * Disable depth test
	*/

	GL::Disable (GL_DEPTH_TEST);

	/*
	 * Enable blend only for source
	*/

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

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

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (settings));

	/*
	 * Draw volumetric light
	*/

	renderLightObject->DrawGeometry ();
}

void DirectionalVolumetricLightRenderPass::EndDirectionalLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();

	GL::Disable (GL_STENCIL_TEST);
}

std::vector<PipelineAttribute> DirectionalVolumetricLightRenderPass::GetCustomAttributes (const Camera* camera,
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

	PipelineAttribute lightDirection;
	PipelineAttribute lightColor;
	PipelineAttribute lightIntensity;

	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	lightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	lightIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lightDirection.name = "lightDirection";
	lightColor.name = "lightColor";
	lightIntensity.name = "lightIntensity";

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);

	glm::mat3 viewMatrix = glm::mat3_cast (camera->GetRotation ());
	lightDir = glm::normalize (viewMatrix * lightDir);

	lightDirection.value = lightDir;
	lightColor.value = renderLightObject->GetLightColor ().ToVector3 ();
	lightIntensity.value.x = renderLightObject->GetLightIntensity ();

	attributes.push_back (lightDirection);
	attributes.push_back (lightColor);
	attributes.push_back (lightIntensity);

	return attributes;
}
