#include "RSMRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

void RSMRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for not animated objects
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void RSMRenderPass::Clear ()
{

}

bool RSMRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return true;
}

RenderVolumeCollection* RSMRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind framebuffer for writing
	*/

	auto framebuffer = rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");
	framebuffer->BindForWriting ();

	/*
	 * Lock post-process shader
	*/

	Pipeline::LockShader (_shaderView);

	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * Enable color blending
	*/

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_ONE, GL_ONE);

	/*
	 * Disable depth test
	*/

	GL::Disable (GL_DEPTH_TEST);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("GBuffer")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("IndirectMap")->GetCustomAttributes ());

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);

	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();

	return rvc;
}
