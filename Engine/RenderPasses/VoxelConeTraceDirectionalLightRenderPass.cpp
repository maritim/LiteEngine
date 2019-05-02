#include "VoxelConeTraceDirectionalLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Lighting/LightsManager.h"

#include "Systems/Settings/SettingsManager.h"

VoxelConeTraceDirectionalLightRenderPass::VoxelConeTraceDirectionalLightRenderPass () :
	_shadowShaderName ("VOXEL_CONE_TRACE_SHADOW_MAP_DIRECTIONAL_LIGHT"),
	_indirectIntensity (0.0f)
{

}

void VoxelConeTraceDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceVertex.glsl",
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceFragment.glsl");

	/*
	 * Initialize voxel cone trace directional light settings
	*/

	InitSettings ();
}

void VoxelConeTraceDirectionalLightRenderPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update voxel cone trace directional light indirect intensity
	*/

	if (name == "vct_indirect_intensity") {
		_indirectIntensity = SettingsManager::Instance ()->GetValue<float> ("vct_indirect_intensity", _indirectIntensity);
	}
}

void VoxelConeTraceDirectionalLightRenderPass::Clear ()
{
	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void VoxelConeTraceDirectionalLightRenderPass::LockShader (const VolumetricLight* volumetricLight)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shadowShaderName));
}

std::vector<PipelineAttribute> VoxelConeTraceDirectionalLightRenderPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ambientLightColor;
	PipelineAttribute indirectIntensity;

	ambientLightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	indirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ambientLightColor.name = "ambientLightColor";
	indirectIntensity.name = "indirectIntensity";

	ambientLightColor.value = LightsManager::Instance ()->GetAmbientLightColor ().ToVector3 ();
	indirectIntensity.value.x = _indirectIntensity;

	attributes.push_back (ambientLightColor);
	attributes.push_back (indirectIntensity);

	return attributes;
}

void VoxelConeTraceDirectionalLightRenderPass::InitSettings ()
{
	/*
	 * Initialize voxel cone trace directional light indirect intensity
	*/

	_indirectIntensity = SettingsManager::Instance ()->GetValue<float> ("vct_indirect_intensity", _indirectIntensity);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("vct_indirect_intensity", this);
}

void VoxelConeTraceDirectionalLightRenderPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("vct_indirect_intensity", this);
}
