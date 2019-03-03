#include "ReflectiveShadowMapDirectionalLightContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Settings/SettingsManager.h"

ReflectiveShadowMapDirectionalLightContainerRenderSubPass::ReflectiveShadowMapDirectionalLightContainerRenderSubPass () :
	_radius (0.0f),
	_intensity (0.0f),
	_shadowShaderName ("REFLECTIVE_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void ReflectiveShadowMapDirectionalLightContainerRenderSubPass::Init ()
{
	/*
	 * Shader for directional light with shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/ReflectiveShadowMap/reflectiveDeferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveDeferredDirVolShadowMapLightFragment.glsl");

	/*
	 * Initialize reflective shadow map directional light settings
	*/

	InitSettings ();
}

void ReflectiveShadowMapDirectionalLightContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update reflective shadow map sample radius
	*/

	if (name == "rsm_radius") {
		_radius = SettingsManager::Instance ()->GetValue<float> ("rsm_radius", _radius);
	}

	/*
	 * Update reflective shadow map indirect light intensity
	*/

	if (name == "rsm_intensity") {
		_intensity = SettingsManager::Instance ()->GetValue<float> ("rsm_intensity", _intensity);
	}
}

void ReflectiveShadowMapDirectionalLightContainerRenderSubPass::Clear ()
{
	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void ReflectiveShadowMapDirectionalLightContainerRenderSubPass::LockShader (const VolumetricLight* volumetricLight)
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

std::vector<PipelineAttribute> ReflectiveShadowMapDirectionalLightContainerRenderSubPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmRadius;
	PipelineAttribute rsmIntensity;

	rsmRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmRadius.name = "rsmRadius";
	rsmIntensity.name = "rsmIntensity";

	rsmRadius.value.x = _radius;
	rsmIntensity.value.x = _intensity;

	attributes.push_back (rsmRadius);
	attributes.push_back (rsmIntensity);

	return attributes;
}

void ReflectiveShadowMapDirectionalLightContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize reflective shadow map sample radius
	*/

	_radius = SettingsManager::Instance ()->GetValue<float> ("rsm_radius", _radius);

	/*
	 * Initialize reflective shadow map indirect light intensity
	*/

	_intensity = SettingsManager::Instance ()->GetValue<float> ("rsm_intensity", _intensity);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("rsm_radius", this);
	SettingsManager::Instance ()->Attach ("rsm_intensity", this);
}

void ReflectiveShadowMapDirectionalLightContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("rsm_radius", this);
	SettingsManager::Instance ()->Detach ("rsm_intensity", this);
}
