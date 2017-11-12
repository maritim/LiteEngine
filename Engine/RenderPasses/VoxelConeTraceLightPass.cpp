#include "VoxelConeTraceLightPass.h"

#include "Lighting/VolumetricLight.h"
#include "Lighting/LightsManager.h"

#include "Wrappers/OpenGL/GL.h"

#include "VoxelVolume.h"
#include "GBuffer.h"

VoxelConeTraceLightPass::~VoxelConeTraceLightPass ()
{

}

void VoxelConeTraceLightPass::Init ()
{

}

RenderVolumeCollection* VoxelConeTraceLightPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Voxel cone trace light pass
	*/

	LightPass (scene, camera, rvc);

	return rvc;
}

void VoxelConeTraceLightPass::LightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Directional light pass, the only light pass with integrated
	* cone tracing pass at the moment
	*/

	DirectionalLightPass (scene, camera, rvc);
}

void VoxelConeTraceLightPass::DirectionalLightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	GL::Disable (GL_DEPTH_TEST);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	for (std::size_t i = 0; i<LightsManager::Instance ()->GetDirectionalLightsCount (); i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetDirectionalLight (i);

		if (!volumetricLight->IsActive ()) {
			continue;
		}

		voxelVolume->BindForReading ();

		volumetricLight->GetLightRenderer ()->Draw (scene, camera, rvc);
	}

	GL::Disable (GL_BLEND);
	GL::Enable (GL_DEPTH_TEST);
}
