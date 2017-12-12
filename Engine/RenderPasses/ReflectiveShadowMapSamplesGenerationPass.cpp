#include "ReflectiveShadowMapSamplesGenerationPass.h"

#include "Core/Console/Console.h"

ReflectiveShadowMapSamplesGenerationPass::ReflectiveShadowMapSamplesGenerationPass ()
	: _reflectiveShadowMapSamplesVolume (new ReflectiveShadowMapSamplesVolume ())
{

}

void ReflectiveShadowMapSamplesGenerationPass::Init ()
{
	if (!_reflectiveShadowMapSamplesVolume->Init(50)) {
		Console::LogError("Reflective shadow map samples cannot be initialized! It is not possible to continue the process. End now!");
		exit(REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT);
	}
}

RenderVolumeCollection* ReflectiveShadowMapSamplesGenerationPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	return rvc->Insert ("ReflectiveShadowMapSamplesVolume", _reflectiveShadowMapSamplesVolume);
}
