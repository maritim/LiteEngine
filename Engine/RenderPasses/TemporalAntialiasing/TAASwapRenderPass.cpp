#include "TAASwapRenderPass.h"

#include "TAAMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void TAASwapRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* TAASwapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	auto taaMapVolume = (TAAMapVolume*) rvc->GetRenderVolume ("TAAMapVolume");
	auto currentTAAMapVolume = (TAAMapVolume*) rvc->GetRenderVolume ("PostProcessMapVolume");

	/*
	 * Swap ping-pong buffers
	*/

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();
	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);

	glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

	taaMapVolume->SetViewProjectionMatrix (viewProjectionMatrix);

	/*
	 * Blit current framebuffer into last history framebuffer
	*/

	currentTAAMapVolume->BindForBliting ();
	taaMapVolume->BindToBlit ();

	glm::ivec2 currentTAAMapSize = currentTAAMapVolume->GetSize ();
	glm::ivec2 taaMapSize = taaMapVolume->GetSize ();

	GL::BlitFramebuffer (0, 0, currentTAAMapSize.x, currentTAAMapSize.y,
		0, 0, taaMapSize.x, taaMapSize.y,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	return rvc;
}

void TAASwapRenderPass::Clear ()
{

}

bool TAASwapRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if temporal anti-aliasing is enabled
	*/

	return settings.taa_enabled;
}
