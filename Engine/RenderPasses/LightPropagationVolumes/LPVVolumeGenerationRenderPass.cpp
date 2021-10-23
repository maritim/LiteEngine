#include "LPVVolumeGenerationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

LPVVolumeGenerationRenderPass::LPVVolumeGenerationRenderPass () :
	_lpvVolume (new LPVVolume ()),
	_lpvGeometryVolume (new LPVGeometryVolume ()),
	_lpvSampleCountVolume (new LPVSampleCountVolume ())
{

}

LPVVolumeGenerationRenderPass::~LPVVolumeGenerationRenderPass ()
{
	delete _lpvSampleCountVolume;
	delete _lpvGeometryVolume;
	delete _lpvVolume;
}

void LPVVolumeGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize light propagation volume
	*/

	InitLPVVolume (settings);
}

void LPVVolumeGenerationRenderPass::Clear ()
{
	/*
	 * Clear post processing volume
	*/

	_lpvSampleCountVolume->Clear ();
	_lpvGeometryVolume->Clear ();
	_lpvVolume->Clear ();
}

RenderVolumeCollection* LPVVolumeGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update LPV volume
	*/

	UpdateLPVVolume (settings);

	/*
	 * Update voxel volume based on scene bounding box
	*/

	UpdateLPVVolumeBoundingBox (renderScene);

	_lpvSampleCountVolume->ClearVolume ();
	_lpvGeometryVolume->ClearVolume ();
	_lpvVolume->ClearVolume ();

	return rvc->Insert ("LightPropagationVolume", _lpvVolume)
		->Insert ("LPVGeometryVolume", _lpvGeometryVolume)
		->Insert ("LPVSampleCountVolume", _lpvSampleCountVolume);
}

bool LPVVolumeGenerationRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVVolumeGenerationRenderPass::UpdateLPVVolumeBoundingBox (const RenderScene* renderScene)
{
	auto& volume = renderScene->GetBoundingBox ();

	glm::vec3 minVertex = volume.minVertex;
	glm::vec3 maxVertex = volume.maxVertex;

	_lpvVolume->UpdateBoundingBox (minVertex, maxVertex);
	_lpvGeometryVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void LPVVolumeGenerationRenderPass::InitLPVVolume (const RenderSettings& settings)
{
	if (!_lpvVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}

	if (!_lpvGeometryVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}

	if (!_lpvSampleCountVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}
}

void LPVVolumeGenerationRenderPass::UpdateLPVVolume (const RenderSettings& settings)
{
	if (_lpvVolume->GetVolumeSize () != settings.lpv_volume_size) {

		/*
		 * Clear voxel volume
		*/

		_lpvSampleCountVolume->Clear ();
		_lpvGeometryVolume->Clear ();
		_lpvVolume->Clear ();

		/*
		 * Initialize voxel volume
		*/

		InitLPVVolume (settings);
	}
}
