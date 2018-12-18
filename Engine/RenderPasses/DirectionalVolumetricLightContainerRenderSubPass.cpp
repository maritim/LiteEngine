#include "DirectionalVolumetricLightContainerRenderSubPass.h"

#include "Renderer/Pipeline.h"

DirectionalVolumetricLightContainerRenderSubPass::~DirectionalVolumetricLightContainerRenderSubPass ()
{

}

RenderVolumeCollection* DirectionalVolumetricLightContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	StartDirectionalLightPass (rvc);

	/*
	 * Draw volumetric lights
	*/

	DirectionalLightPass (scene, camera, rvc);

	/*
	 * End directional light pass
	*/

	EndDirectionalLightPass ();

	return rvc;
}

bool DirectionalVolumetricLightContainerRenderSubPass::IsAvailable (const VolumetricLight*) const
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

void DirectionalVolumetricLightContainerRenderSubPass::DirectionalLightPass (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
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

	VolumetricLight* volumetricLight = GetVolumetricLight (rvc);

	/*
	 * Lock shader for volumetric light draw
	*/

	LockShader (volumetricLight);

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
	 * Draw volumetric light
	*/

	volumetricLight->GetLightRenderer ()->Draw (scene, camera, rvc);
}

void DirectionalVolumetricLightContainerRenderSubPass::EndDirectionalLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}
