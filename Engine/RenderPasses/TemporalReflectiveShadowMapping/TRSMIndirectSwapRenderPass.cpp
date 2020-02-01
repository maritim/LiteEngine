#include "TRSMIndirectSwapRenderPass.h"

#include "TRSMIndirectLightMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void TRSMIndirectSwapRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* TRSMIndirectSwapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	auto rsmLastIndirectMapVolume = (TRSMIndirectLightMapVolume*) rvc->GetRenderVolume ("LastIndirectMap");
	auto rsmIndirectMapVolume = (TRSMIndirectLightMapVolume*) rvc->GetRenderVolume ("IndirectMap");

	/*
	 * Swap ping-pong buffers
	*/

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();
	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);

	glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

	rsmLastIndirectMapVolume->SetViewProjectionMatrix (viewProjectionMatrix);

	/*
	 * Blit current framebuffer into last history framebuffer
	*/

	rsmIndirectMapVolume->BindForBliting ();
	rsmLastIndirectMapVolume->BindToBlit ();

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);

	GL::BlitFramebuffer (0, 0, resolution.x, resolution.y,
		0, 0, resolution.x, resolution.y,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	return rvc;
}

void TRSMIndirectSwapRenderPass::Clear ()
{

}

bool TRSMIndirectSwapRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute skybox render sub pass
	*/

	return true;
}
