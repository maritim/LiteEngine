#include "DirectionalVolumetricLightContainerRenderSubPass.h"

#include "Renderer/Pipeline.h"

DirectionalVolumetricLightContainerRenderSubPass::~DirectionalVolumetricLightContainerRenderSubPass ()
{

}

RenderVolumeCollection* DirectionalVolumetricLightContainerRenderSubPass::Execute (const RenderScene* renderScene, const Camera* camera,
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

bool DirectionalVolumetricLightContainerRenderSubPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute directional volumetric light render sub pass
	*/

	return true;
}

void DirectionalVolumetricLightContainerRenderSubPass::StartDirectionalLightPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto lightAccumulationVolume = rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForWriting ();
}

void DirectionalVolumetricLightContainerRenderSubPass::DirectionalLightPass (const RenderScene* renderScene, const Camera* camera,
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

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (rvc));

	/*
	 * Send custom attributes
	*/

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (settings));

	/*
	 * Draw volumetric light
	*/

	renderLightObject->Draw ();
}

void DirectionalVolumetricLightContainerRenderSubPass::EndDirectionalLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> DirectionalVolumetricLightContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc) const
{
	std::vector<PipelineAttribute> attributes;

	for (RenderVolumeI* renderVolume : *rvc) {
		std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	return attributes;
}
