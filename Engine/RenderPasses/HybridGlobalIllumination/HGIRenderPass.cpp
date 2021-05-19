#include "HGIRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "RenderPasses/FramebufferRenderVolume.h"

void HGIRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for not animated objects
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/HybridGlobalIllumination/hybridGlobalIlluminationFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void HGIRenderPass::Clear ()
{

}

bool HGIRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return true;
}

RenderVolumeCollection* HGIRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind framebuffer for writing
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("ResultFramebufferRenderVolume");

	resultVolume->GetFramebufferView ()->Activate ();

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
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("directLightMap")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("HybridIndirectDiffuseMap")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("indirectSpecularMap")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("ambientOcclusionMap")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (camera, settings, rvc));

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

std::vector<PipelineAttribute> HGIRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute indirectSpecularEnabled;
	PipelineAttribute aoEnabled;

	indirectSpecularEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	aoEnabled.type = PipelineAttribute::AttrType::ATTR_1I;

	indirectSpecularEnabled.name = "indirectSpecularEnabled";
	aoEnabled.name = "aoEnabled";

	indirectSpecularEnabled.value.x = settings.indirect_specular_enabled;
	aoEnabled.value.x = settings.hgi_ao_enabled;

	attributes.push_back (indirectSpecularEnabled);
	attributes.push_back (aoEnabled);

	return attributes;
}
