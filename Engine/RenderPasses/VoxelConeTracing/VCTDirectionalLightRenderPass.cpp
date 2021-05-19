#include "VCTDirectionalLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void VCTDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shadowShader = Resources::LoadShader ({
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingVertex.glsl",
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shadowShader);
}

void VCTDirectionalLightRenderPass::Clear ()
{

}

void VCTDirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (_shadowShaderView);
}

std::vector<PipelineAttribute> VCTDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectSpecularEnabled;
	PipelineAttribute subsurfaceScatteringEnabled;
	PipelineAttribute originBias;

	indirectSpecularEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	subsurfaceScatteringEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	originBias.type = PipelineAttribute::AttrType::ATTR_1F;

	indirectSpecularEnabled.name = "indirectSpecularEnabled";
	subsurfaceScatteringEnabled.name = "subsurfaceScatteringEnabled";
	originBias.name = "originBias";

	indirectSpecularEnabled.value.x = settings.indirect_specular_enabled;
	subsurfaceScatteringEnabled.value.x = settings.subsurface_scattering_enabled;
	originBias.value.x = settings.vct_origin_bias;

	attributes.push_back (indirectSpecularEnabled);
	attributes.push_back (subsurfaceScatteringEnabled);
	attributes.push_back (originBias);

	return attributes;
}
