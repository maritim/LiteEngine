#include "DirectionalLightExponentialShadowMapContainerRenderSubPass.h"

#include "ExponentialCascadedShadowMapDirectionalLightVolume.h"

#include "Managers/ShaderManager.h"

#include "Core/Console/Console.h"

#include "Systems/Settings/SettingsManager.h"

DirectionalLightExponentialShadowMapContainerRenderSubPass::DirectionalLightExponentialShadowMapContainerRenderSubPass () :
	DirectionalLightShadowMapContainerRenderSubPass (),
	_exponential (0)
{
	_staticShaderName = "STATIC_EXPONENTIAL_SHADOW_MAP";
	_animationShaderName = "ANIMATION_EXPONENTIAL_SHADOW_MAP";

	delete _volume;
	_volume = new ExponentialCascadedShadowMapDirectionalLightVolume ();
}

void DirectionalLightExponentialShadowMapContainerRenderSubPass::Init ()
{
	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertex.glsl",
		"Assets/Shaders/ShadowMap/exponentialShadowMapFragment.glsl");

	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertexAnimation.glsl",
		"Assets/Shaders/ShadowMap/exponentialShadowMapFragment.glsl");

	/*
	 * Initialize settings
	*/

	InitSettings ();

	/*
	 * Initialize shadow map volume
	*/

	InitShadowMapVolume ();
}

void DirectionalLightExponentialShadowMapContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	/*
	 *
	*/

	DirectionalLightShadowMapContainerRenderSubPass::Notify (sender, args);

	std::string name = args.GetName ();

	/*
	 * Update directional light exponential shadow map
	*/

	if (name == "esm_exponential") {
		_exponential = SettingsManager::Instance ()->GetValue<int> ("esm_exponential", _exponential);

		auto volume = (ExponentialCascadedShadowMapDirectionalLightVolume*) _volume;
		volume->SetExponential (_exponential);
	}
}

std::vector<PipelineAttribute> DirectionalLightExponentialShadowMapContainerRenderSubPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute exponential;

	exponential.type = PipelineAttribute::AttrType::ATTR_1F;

	exponential.name = "exponential";

	exponential.value.x = _exponential;

	attributes.push_back (exponential);

	return attributes;
}

void DirectionalLightExponentialShadowMapContainerRenderSubPass::InitSettings ()
{
	/*
	 * 
	*/

	DirectionalLightShadowMapContainerRenderSubPass::InitSettings ();

	/*
	 * Initialize directional exponential light shadow map
	*/

	_exponential = SettingsManager::Instance ()->GetValue<int> ("esm_exponential", _exponential);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("esm_exponential", this);
}

void DirectionalLightExponentialShadowMapContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("esm_exponential", this);

	/*
	 *
	*/

	DirectionalLightShadowMapContainerRenderSubPass::ClearSettings ();
}

void DirectionalLightExponentialShadowMapContainerRenderSubPass::InitShadowMapVolume ()
{
	/*
	 * Initialize shadow map volume
	*/

	if (!_volume->Init (_cascades, _resolution)) {
		Console::LogError (std::string () + "Exponential Shadow map cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (EXPONENTIAL_SHADOW_MAP_FBO_NOT_INIT);
	}

	for (std::size_t index = 0; index < _cascades; index ++) {
		_volume->SetLightCamera (index, new OrthographicCamera ());
	}

	auto volume = (ExponentialCascadedShadowMapDirectionalLightVolume*) _volume;
	volume->SetExponential (_exponential);
}
