#include "TemporalFilterSwapRenderPass.h"

#include "TemporalFilterMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void TemporalFilterSwapRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* TemporalFilterSwapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	auto temporalFilterMapVolume = (TemporalFilterMapVolume*) rvc->GetRenderVolume ("TemporalFilterMapVolume");
	auto currentTemporalFilterMapVolume = (TemporalFilterMapVolume*) rvc->GetRenderVolume (GetPostProcessMapVolumeName ());

	/*
	 * Swap ping-pong buffers
	*/

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();
	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);

	glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

	temporalFilterMapVolume->SetViewProjectionMatrix (viewProjectionMatrix);

	/*
	 * Blit current framebuffer into last history framebuffer
	*/

	temporalFilterMapVolume->GetFramebufferView ()->Activate ();
	currentTemporalFilterMapVolume->GetFramebufferView ()->ActivateSource ();

	auto currentTemporalFilterMapSize = currentTemporalFilterMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();
	auto temporalFilterMapSize = temporalFilterMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	GL::BlitFramebuffer (0, 0, currentTemporalFilterMapSize.width, currentTemporalFilterMapSize.height,
		0, 0, temporalFilterMapSize.width, temporalFilterMapSize.height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	return rvc;
}

void TemporalFilterSwapRenderPass::Clear ()
{

}

std::string TemporalFilterSwapRenderPass::GetPostProcessMapVolumeName () const
{
	return "PostProcessMapVolume";
}
