#include "DirectionalLightExponentialShadowMapRenderPass.h"

#include "ExponentialCascadedShadowMapDirectionalLightVolume.h"

#include "Managers/ShaderManager.h"

#include "Core/Console/Console.h"

#include "Systems/Settings/SettingsManager.h"

DirectionalLightExponentialShadowMapRenderPass::DirectionalLightExponentialShadowMapRenderPass () :
	DirectionalLightShadowMapRenderPass (),
	_exponential (0)
{
	_staticShaderName = "STATIC_EXPONENTIAL_SHADOW_MAP";
	_animationShaderName = "ANIMATION_EXPONENTIAL_SHADOW_MAP";

	delete _volume;
	_volume = new ExponentialCascadedShadowMapDirectionalLightVolume ();
}

void DirectionalLightExponentialShadowMapRenderPass::Init ()
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
}

std::vector<PipelineAttribute> DirectionalLightExponentialShadowMapRenderPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute exponential;

	exponential.type = PipelineAttribute::AttrType::ATTR_1F;

	exponential.name = "exponential";

	exponential.value.x = _exponential;

	attributes.push_back (exponential);

	return attributes;
}

void DirectionalLightExponentialShadowMapRenderPass::InitShadowMapVolume (const RenderLightObject* renderLightObject)
{
	/*
	 * Initialize shadow map volume
	*/

	// if (!_volume->Init (_cascades, _resolution)) {
	// 	Console::LogError (std::string () + "Exponential Shadow map cannot be initialized!" +
	// 		" It is not possible to continue the process. End now!");
	// 	exit (EXPONENTIAL_SHADOW_MAP_FBO_NOT_INIT);
	// }

	// for (std::size_t index = 0; index < _cascades; index ++) {
	// 	_volume->SetLightCamera (index, new OrthographicCamera ());
	// }

	// auto volume = (ExponentialCascadedShadowMapDirectionalLightVolume*) _volume;
	// volume->SetExponential (_exponential);
}
