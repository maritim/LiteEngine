#include "SSAOSamplesGenerationContainerRenderSubPass.h"

#include "Core/Console/Console.h"

SSAOSamplesGenerationContainerRenderSubPass::SSAOSamplesGenerationContainerRenderSubPass () :
	_ssaoSamplesVolume (new SSAOSamplesVolume ())
{

}

SSAOSamplesGenerationContainerRenderSubPass::~SSAOSamplesGenerationContainerRenderSubPass ()
{
	delete _ssaoSamplesVolume;
}

void SSAOSamplesGenerationContainerRenderSubPass::Init ()
{
	if (!_ssaoSamplesVolume->Init (32)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_SAMPLES_NOT_INIT);
	}
}

RenderVolumeCollection* SSAOSamplesGenerationContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	return rvc->InsertScoped ("SSAOSamplesVolume", _ssaoSamplesVolume);
}

bool SSAOSamplesGenerationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * TODO: Check if SSAO is enabled
	*/

	return true;
}
