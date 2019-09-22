#include "AmbientLightRenderPass.h"

AmbientLightRenderPass::AmbientLightRenderPass () :
	_ambientLightVolume (new AmbientLightVolume ())
{

}

AmbientLightRenderPass::~AmbientLightRenderPass ()
{
	delete _ambientLightVolume;
}

void AmbientLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Do nothing
	*/
}

RenderVolumeCollection* AmbientLightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	auto lightObject = renderScene->GetRenderAmbientLightObject ();

	bool aoEnabled = settings.ssao_enabled;

	_ambientLightVolume->SetAmbientLight (
		lightObject != nullptr && lightObject->isActive ? lightObject->intensity : 1.0f,
		lightObject != nullptr && lightObject->isActive ? lightObject->color : Color::Black,
		aoEnabled
	);

	return rvc->Insert ("AmbientLightVolume", _ambientLightVolume);
}

bool AmbientLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute ambient light render pass
	*/

	return true;
}

void AmbientLightRenderPass::Clear ()
{
	/*
	 * Do nothing
	*/
}
