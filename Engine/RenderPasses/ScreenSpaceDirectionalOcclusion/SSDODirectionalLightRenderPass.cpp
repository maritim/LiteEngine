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
	PipelineAttribute ssdoRayShadow;

	indirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoRayShadow.type = PipelineAttribute::AttrType::ATTR_1I;

	indirectIntensity.name = "indirectIntensity";
	ssdoRayShadow.name = "ssdoRayShadow";

	indirectIntensity.value.x = settings.ssdo_indirect_intensity;
	ssdoRayShadow.value.x = settings.ssdo_ray_shadow;

	attributes.push_back (indirectIntensity);
	attributes.push_back (ssdoRayShadow);

	return attributes;
}
