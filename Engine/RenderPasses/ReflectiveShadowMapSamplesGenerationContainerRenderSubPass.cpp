#include "ReflectiveShadowMapSamplesGenerationContainerRenderSubPass.h"

#include "Core/Console/Console.h"

ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::ReflectiveShadowMapSamplesGenerationContainerRenderSubPass ()
	: _reflectiveShadowMapSamplesVolume (new ReflectiveShadowMapSamplesVolume ())
{

}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::Init ()
{
	if (!_reflectiveShadowMapSamplesVolume->Init(50)) {
		Console::LogError(std::string () + "Reflective shadow map samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit(REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT);
	}
}

RenderVolumeCollection* ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	return rvc->InsertScoped ("ReflectiveShadowMapSamplesVolume", _reflectiveShadowMapSamplesVolume);
}

bool ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::IsAvailable (const VolumetricLight* volumetricLight) const
{
	/*
	 * Always execute reflective shadow map accumulation sub pass
	*/

	return true;
}
