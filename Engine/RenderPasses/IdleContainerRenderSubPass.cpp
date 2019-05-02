#include "IdleContainerRenderSubPass.h"

#include "LightAccumulationVolume.h"

IdleContainerRenderSubPass::IdleContainerRenderSubPass () :
	_postProcessMapVolume (new PostProcessMapVolume ())
{

}

IdleContainerRenderSubPass::~IdleContainerRenderSubPass ()
{
	delete _postProcessMapVolume;
}

void IdleContainerRenderSubPass::Init (const RenderSettings& settings)
{
	InitVolume (settings);
}

RenderVolumeCollection* IdleContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateVolume (settings);

	/*
	 * Get frame buffer from render volume collection
	*/

	LightAccumulationVolume* lightAccumulationVolume = (LightAccumulationVolume*) rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForBliting ();

	_postProcessMapVolume->BindToBlit ();

	GL::BlitFramebuffer (settings.viewport.x, settings.viewport.y, 
		settings.viewport.width, settings.viewport.height,
		settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	return rvc->Insert ("PostProcessMapVolume", _postProcessMapVolume);
}

bool IdleContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute idle container render sub pass
	*/

	return true;
}

void IdleContainerRenderSubPass::Clear ()
{
	/*
	 * Clear post process map volume
	*/

	_postProcessMapVolume->Clear ();
}

void IdleContainerRenderSubPass::UpdateVolume (const RenderSettings& settings)
{
	Framebuffer framebuffer = settings.framebuffer;

	glm::ivec2 size = _postProcessMapVolume->GetSize ();

	if ((std::size_t) size.x != framebuffer.width || (std::size_t) size.y != framebuffer.height) {

		/*
		 * Clear post process volume
		*/

		_postProcessMapVolume->Clear ();

		/*
		 * Initialize post process volume
		*/

		InitVolume (settings);
	}
}

void IdleContainerRenderSubPass::InitVolume (const RenderSettings& settings)
{
	if (!_postProcessMapVolume->Init (glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height))) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}