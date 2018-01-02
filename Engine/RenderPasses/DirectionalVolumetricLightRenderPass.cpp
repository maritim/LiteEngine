#include "DirectionalVolumetricLightRenderPass.h"

#include "Renderer/Pipeline.h"

DirectionalVolumetricLightRenderPass::~DirectionalVolumetricLightRenderPass ()
{

}

RenderVolumeCollection* DirectionalVolumetricLightRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

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

bool DirectionalVolumetricLightRenderPass::IsAvailable (const VolumetricLight*) const
{
	/*
	 * Always execute directional volumetric light render sub pass
	*/

	return true;
}

void DirectionalVolumetricLightRenderPass::DirectionalLightPass (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
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

void DirectionalVolumetricLightRenderPass::EndDirectionalLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}
