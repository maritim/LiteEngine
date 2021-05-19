#include "HybridRSMAmbientOcclusionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "RenderPasses/GBuffer.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "HGIStatisticsObject.h"

#include "Utils/Extensions/MathExtend.h"

void HybridRSMAmbientOcclusionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for not animated objects
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/HybridGlobalIllumination/hybridRSMAmbientOcclusionFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void HybridRSMAmbientOcclusionRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

bool HybridRSMAmbientOcclusionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	return settings.hgi_ao_enabled && VolumetricLightRenderPassI::IsAvailable (renderScene, camera, settings, rvc);
}

bool HybridRSMAmbientOcclusionRenderPass::IsAvailable (const RenderLightObject*) const
{
	return true;
}

RenderVolumeCollection* HybridRSMAmbientOcclusionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Bind framebuffer for writing
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("ambientOcclusionMap");

	resultVolume->GetFramebufferView ()->Activate ();

	/*
	 * Update statistics object
	*/

	auto hgiStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

	hgiStatisticsObject->hgiRSMAmbientOcclusionMapVolume = resultVolume;

	/*
	 * Lock post-process shader
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
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("NoiseMapVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("SSAOSamplesVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (camera, settings, renderLightObject));

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

std::vector<PipelineAttribute> HybridRSMAmbientOcclusionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, const RenderLightObject* renderLightObject) const
{
	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;
	PipelineAttribute hgiAOBlend;
	PipelineAttribute lightDirection;
	PipelineAttribute temporalFilterEnabled;

	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiAOBlend.type = PipelineAttribute::AttrType::ATTR_1F;
	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	temporalFilterEnabled.type = PipelineAttribute::AttrType::ATTR_1I;

	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";
	hgiAOBlend.name = "hgiAOBlend";
	lightDirection.name = "lightDirection";
	temporalFilterEnabled.name = "temporalFilterEnabled";

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);
	glm::mat3 viewMatrix = glm::mat3_cast (camera->GetRotation ());
	lightDir = glm::normalize (viewMatrix * lightDir);

	ssaoRadius.value.x = settings.hgi_ao_radius;
	ssaoBias.value.x = settings.hgi_ao_bias;
	hgiAOBlend.value.x = settings.hgi_ao_blend;
	lightDirection.value = lightDir;
	temporalFilterEnabled.value.x = settings.hgi_temporal_filter_enabled;

	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);
	attributes.push_back (hgiAOBlend);
	attributes.push_back (lightDirection);
	attributes.push_back (temporalFilterEnabled);

	return attributes;
}
