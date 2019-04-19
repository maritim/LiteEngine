#include "ExponentialShadowMapBlurContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "RenderPasses/ShadowMap/ExponentialCascadedShadowMapDirectionalLightVolume.h"

#include "Renderer/Pipeline.h"

#include "Systems/Settings/SettingsManager.h"

ExponentialShadowMapBlurContainerRenderSubPass::ExponentialShadowMapBlurContainerRenderSubPass () :
	_shaders { "SHADOW_MAP_CASCADED_HORIZONTAL_BLUR", "SHADOW_MAP_CASCADED_VERTICAL_BLUR" },
	_framebuffers (new ExponentialShadowMapBlurVolume* [2])
{
	for (std::size_t index = 0; index < 2; index ++) {
		_framebuffers [index] = new ExponentialShadowMapBlurVolume ();
	}
}

ExponentialShadowMapBlurContainerRenderSubPass::~ExponentialShadowMapBlurContainerRenderSubPass ()
{
	/*
	 * Delete every framebuffer
	*/

	for (std::size_t index = 0; index < 2; index ++) {
		delete _framebuffers [index];
	}

	delete[] _framebuffers;
}

void ExponentialShadowMapBlurContainerRenderSubPass::Init ()
{
	/*
	 * Initialize ping pong shaders
	*/

	ShaderManager::Instance ()->AddShader (_shaders [0],
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/Blur/horizontalGaussianBlurFragment.glsl");

	ShaderManager::Instance ()->AddShader (_shaders [1],
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/Blur/verticalGaussianBlurFragment.glsl");

	/*
	 * Initialize settings
	*/

	InitSettings ();

	/*
	 * Initialize shadow map blur volumes
	*/

	InitShadowMapBlurVolumes ();
}

void ExponentialShadowMapBlurContainerRenderSubPass::Clear ()
{
	/*
	 * Clear framebuffers volumes
	*/

	for (std::size_t index = 0; index < 2; index ++) {
		_framebuffers [index]->Clear ();
	}

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

RenderVolumeCollection* ExponentialShadowMapBlurContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

	auto volume = (ExponentialCascadedShadowMapDirectionalLightVolume*) rvc->GetRenderVolume ("ShadowMapDirectionalLightVolume");

	for (std::size_t cascadeIndex = 0; cascadeIndex < _cascades; cascadeIndex ++) {

		ShadowMapVolume* shadowMap = volume->GetShadowMapVolume (cascadeIndex);

		shadowMap->BindForBliting ();
		_framebuffers [0]->BindToBlit ();

		GL::BlitFramebuffer (0, 0, _resolution.x, _resolution.y, 0, 0, _resolution.x, _resolution.y,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);

		for (std::size_t index = 0; index < 1; index ++) {

			Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaders [0]));

			Blur (_framebuffers [index % 2], _framebuffers [1 - index % 2]);

			Pipeline::UnlockShader ();

			Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaders [1]));

			Blur (_framebuffers [1 - index % 2], _framebuffers [index % 2]);

			Pipeline::UnlockShader ();
		}

		_framebuffers [1]->BindForBliting ();
		shadowMap->BindToBlit ();

		GL::BlitFramebuffer (0, 0, _resolution.x, _resolution.y, 0, 0, _resolution.x, _resolution.y,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	return rvc->Insert ("ShadowMapDirectionalLightVolume", volume);
}

void ExponentialShadowMapBlurContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update directional light shadow map cascades
	*/

	if (name == "sm_cascades") {
		_cascades = SettingsManager::Instance ()->GetValue<int> ("sm_cascades", _cascades);
	}

	/*
	 * Update directional light shadow map resolution
	*/

	if (name == "sm_resolution") {
		_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("sm_resolution", (glm::vec2) _resolution);

		/*
		 * Clear ping pong framebuffers
		*/

		for (std::size_t index = 0; index < 2; index ++) {
			_framebuffers [index]->Clear ();
		}

		/*
		 * Initialize shadow map blur volumes
		*/

		InitShadowMapBlurVolumes ();
	}
}

void ExponentialShadowMapBlurContainerRenderSubPass::Blur (ExponentialShadowMapBlurVolume* fb1, ExponentialShadowMapBlurVolume* fb2)
{
	fb1->BindForReading ();
	fb2->BindForWriting ();

	/*
	 * Set viewport
	*/

	GL::Viewport (0, 0, _resolution.x, _resolution.y);

	/*
	 * Enable color blending
	*/

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes ("", fb1->GetCustomAttributes ());

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

bool ExponentialShadowMapBlurContainerRenderSubPass::IsAvailable (const VolumetricLight* volumetricLight) const
{
	/*
	 * Execute shadow map sub pass only if light is casting shadows
	*/

	return volumetricLight->IsCastingShadows ();
}

void ExponentialShadowMapBlurContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize directional light shadow map cascades
	*/

	_cascades = SettingsManager::Instance ()->GetValue<int> ("sm_cascades", _cascades);

	/*
	 * Initialize directional light shadow map resolution
	*/

	_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("sm_resolution", (glm::vec2) _resolution);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("sm_cascades", this);
	SettingsManager::Instance ()->Attach ("sm_resolution", this);
}

void ExponentialShadowMapBlurContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("sm_cascades", this);
	SettingsManager::Instance ()->Detach ("sm_resolution", this);
}

void ExponentialShadowMapBlurContainerRenderSubPass::InitShadowMapBlurVolumes ()
{
	/*
	 * Initialize ping pong framebuffers
	*/

	for (std::size_t index = 0; index < 2; index ++) {
		if (!_framebuffers [index]->Init (_resolution.x, _resolution.y)) {
			Console::LogError (std::string () + "Exponential shadow map cascade blur volume cannot be initialized!" +
				"It is not possible to continue the process. End now!");
			exit (0);
		}
	}
}
