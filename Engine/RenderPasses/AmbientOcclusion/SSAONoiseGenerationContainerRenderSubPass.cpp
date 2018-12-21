#include "SSAONoiseGenerationContainerRenderSubPass.h"

#include "Core/Console/Console.h"

#define NOISE_TEXTURE_WIDTH 4
#define NOISE_TEXTURE_HEIGHT 4

SSAONoiseGenerationContainerRenderSubPass::SSAONoiseGenerationContainerRenderSubPass () :
	_ssaoNoiseMapVolume (new SSAONoiseMapVolume ())
{

}

SSAONoiseGenerationContainerRenderSubPass::~SSAONoiseGenerationContainerRenderSubPass ()
{
	delete _ssaoNoiseMapVolume;
}

void SSAONoiseGenerationContainerRenderSubPass::Init ()
{
	if (!_ssaoNoiseMapVolume->Init (NOISE_TEXTURE_WIDTH, NOISE_TEXTURE_HEIGHT)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_NOISE_NOT_INIT);
	}
}

RenderVolumeCollection* SSAONoiseGenerationContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	return rvc->InsertScoped ("SSAONoiseMapVolume", _ssaoNoiseMapVolume);
}

bool SSAONoiseGenerationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * TODO: Check if SSAO is enabled
	*/

	return true;
}
