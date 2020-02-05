#include "SSDODirectionalLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void SSDODirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionLightVertex.glsl",
		"Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionLightFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void SSDODirectionalLightRenderPass::Clear ()
{

}

void SSDODirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (_shaderView);
}

std::vector<PipelineAttribute> SSDODirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectIntensity;

	indirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	indirectIntensity.name = "indirectIntensity";

	indirectIntensity.value.x = settings.vct_indirect_intensity;

	attributes.push_back (indirectIntensity);

	return attributes;
}
