#include "HGIIndirectSpecularLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "HGIStatisticsObject.h"

#include "RenderPasses/GBuffer.h"

bool HGIIndirectSpecularLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	return true;
}

void HGIIndirectSpecularLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/HybridGlobalIllumination/hybridGlobalIlluminationIndirectSpecularFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* HGIIndirectSpecularLightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("indirectSpecularMap");

	resultVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	 * Update statistics object
	*/

	auto hgiStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

	hgiStatisticsObject->hgiIndirectSpecularMapVolume = resultVolume;

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (_shaderView);

	/*
	 * Update matrices
	*/

	Pipeline::CreateProjection (((GBuffer*) rvc->GetRenderVolume ("GBuffer"))->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);
	Pipeline::SetObjectTransform (Transform::Default ());

	Pipeline::UpdateMatrices (nullptr);

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
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("ReflectiveShadowMapVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("HybridSSRPositionMap")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("directLightMap")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (settings));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);

	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	return rvc;
}

void HGIIndirectSpecularLightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> HGIIndirectSpecularLightRenderPass::GetCustomAttributes (const RenderSettings& settings)
{
	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmThickness;
	PipelineAttribute rsmIntensity;
	PipelineAttribute ssrIntensity;

	rsmThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmThickness.name = "rsmThickness";
	rsmIntensity.name = "rsmIntensity";
	ssrIntensity.name = "ssrIntensity";

	rsmThickness.value.x = settings.hgi_rsm_thickness;
	rsmIntensity.value.x = settings.hgi_rsm_indirect_specular_intensity;
	ssrIntensity.value.x = settings.hgi_ssr_indirect_specular_intensity;

	attributes.push_back (rsmThickness);
	attributes.push_back (rsmIntensity);
	attributes.push_back (ssrIntensity);

	return attributes;
}
