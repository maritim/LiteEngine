#include "ExponentialShadowMapBlurRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "RenderPasses/ShadowMap/ExponentialCascadedShadowMapDirectionalLightVolume.h"

#include "Renderer/Pipeline.h"

#include "Systems/Settings/SettingsManager.h"

#include "Core/Console/Console.h"

ExponentialShadowMapBlurRenderPass::ExponentialShadowMapBlurRenderPass () :
	_framebuffers (new ExponentialShadowMapBlurVolume* [2])
{
	for (std::size_t index = 0; index < 2; index ++) {
		_framebuffers [index] = new ExponentialShadowMapBlurVolume ();
	}
}

ExponentialShadowMapBlurRenderPass::~ExponentialShadowMapBlurRenderPass ()
{
	/*
	 * Delete every framebuffer
	*/

	for (std::size_t index = 0; index < 2; index ++) {
		delete _framebuffers [index];
	}

	delete[] _framebuffers;
}

void ExponentialShadowMapBlurRenderPass::Init ()
{
	/*
	 * Initialize ping pong shaders
	*/

	Resource<Shader> horizontalShader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/Blur/horizontalGaussianBlurFragment.glsl"
	});

	_horizontalShaderViewer = RenderSystem::LoadShader (horizontalShader);

	Resource<Shader> verticalShader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/Blur/verticalGaussianBlurFragment.glsl"
	});

	_verticalShaderViewer = RenderSystem::LoadShader (verticalShader);

	/*
	 * Initialize settings
	*/

	InitSettings ();

	/*
	 * Initialize shadow map blur volumes
	*/

	InitShadowMapBlurVolumes ();
}

void ExponentialShadowMapBlurRenderPass::Clear ()
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

RenderVolumeCollection* ExponentialShadowMapBlurRenderPass::Execute (const RenderScene* renderScene, const Camera* camera, RenderVolumeCollection* rvc)
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

			Pipeline::LockShader (_horizontalShaderViewer);

			Blur (_framebuffers [index % 2], _framebuffers [1 - index % 2]);

			Pipeline::UnlockShader ();

			Pipeline::LockShader (_verticalShaderViewer);

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

void ExponentialShadowMapBlurRenderPass::Notify (Object* sender, const SettingsObserverArgs& args)
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

void ExponentialShadowMapBlurRenderPass::Blur (ExponentialShadowMapBlurVolume* fb1, ExponentialShadowMapBlurVolume* fb2)
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

	Pipeline::SendCustomAttributes (nullptr, fb1->GetCustomAttributes ());

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

bool ExponentialShadowMapBlurRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Execute shadow map sub pass only if light is casting shadows
	*/

	return renderLightObject->IsCastingShadows ();
}

void ExponentialShadowMapBlurRenderPass::InitSettings ()
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

void ExponentialShadowMapBlurRenderPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("sm_cascades", this);
	SettingsManager::Instance ()->Detach ("sm_resolution", this);
}

void ExponentialShadowMapBlurRenderPass::InitShadowMapBlurVolumes ()
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
