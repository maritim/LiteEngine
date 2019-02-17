#include "IdleContainerRenderSubPass.h"

#include "LightAccumulationVolume.h"

#include "Systems/Window/Window.h"

IdleContainerRenderSubPass::IdleContainerRenderSubPass () :
	_postProcessMapVolume (new PostProcessMapVolume ())
{

}

IdleContainerRenderSubPass::~IdleContainerRenderSubPass ()
{
	delete _postProcessMapVolume;
}

void IdleContainerRenderSubPass::Init ()
{
	if (!_postProcessMapVolume->Init (Window::GetWidth (), Window::GetHeight ())) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}

RenderVolumeCollection* IdleContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Get frame buffer from render volume collection
	*/

	LightAccumulationVolume* lightAccumulationVolume = (LightAccumulationVolume*) rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForBliting ();

	_postProcessMapVolume->BindToBlit ();

	GL::BlitFramebuffer (0, 0, Window::GetWidth (), Window::GetHeight ()
		, 0, 0, Window::GetWidth (), Window::GetHeight (),
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	return rvc->Insert ("PostProcessMapVolume", _postProcessMapVolume);
}

bool IdleContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
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
