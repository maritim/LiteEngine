#include "ReflectiveShadowMapDirectionalLightVolume.h"

#include "Managers/ShaderManager.h"
#include "Systems/Window/Window.h"
#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

ReflectiveShadowMapDirectionalLightVolume::ReflectiveShadowMapDirectionalLightVolume () :
	_staticShaderName ("STATIC_REFLECTIVE_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_REFLECTIVE_SHADOW_MAP"),
	_shadowMapBuffers (nullptr),
	_shadowMapResolutions (nullptr)
{
	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapFragment.glsl");

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapVertexAnimation.glsl",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapFragment.glsl");
}

ReflectiveShadowMapDirectionalLightVolume::~ReflectiveShadowMapDirectionalLightVolume ()
{
	/*
	 * Delete shadow map buffers
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		delete _shadowMapBuffers [index];
	}

	delete[] _shadowMapBuffers;

	/*
	 * Delete cascaded resolutions
	*/

	delete [] _shadowMapResolutions;
}

bool ReflectiveShadowMapDirectionalLightVolume::Init (std::size_t cascadedLevels)
{
	/*
	 * Initialize cascaded levels
	*/

	_cascadedLevels = cascadedLevels;

	/*
	 * Allocate memory for cascaded levels
	*/

	_shadowMapBuffers = new ReflectiveShadowMapBuffer* [_cascadedLevels];
	_shadowMapResolutions = new std::pair<GLuint, GLuint> [_cascadedLevels];

	/*
	 * Initialize resolutions
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMapResolutions [index].first = REFLECTIVE_SHADOW_MAP_MAX_RESOLUTION_WIDTH;
		_shadowMapResolutions [index].second = REFLECTIVE_SHADOW_MAP_MAX_RESOLUTION_HEIGHT;
	}

	/*
	 * Initialize reflective shadow map buffers
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMapBuffers [index] = new ReflectiveShadowMapBuffer();

		if (_shadowMapBuffers [index]->Init (_shadowMapResolutions [index].first, _shadowMapResolutions [index].second) == false) {
			return false;
		}
	}

	return true;
}

void ReflectiveShadowMapDirectionalLightVolume::BindForShadowMapCatch (std::size_t cascadedLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return;
	}

	/*
	 * Change resolution on viewport as shadow map size
	*/

	GL::Viewport (0, 0, _shadowMapResolutions [cascadedLevel].first, _shadowMapResolutions [cascadedLevel].second);

	/*
	 * Bind reflective shadow map buffer
	*/
	
	_shadowMapBuffers [cascadedLevel]->BindForWriting ();
}

void ReflectiveShadowMapDirectionalLightVolume::EndDrawing ()
{
	Pipeline::UnlockShader ();

	GL::DrawBuffer(GL_NONE);
	GL::ReadBuffer(GL_NONE);

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::size_t windowWidth = Window::GetWidth ();
	std::size_t windowHeight = Window::GetHeight ();

	GL::Viewport(0, 0, windowWidth, windowHeight);
}

void ReflectiveShadowMapDirectionalLightVolume::BindForReading ()
{
	/*
	 * Bind every depth buffer for reading
	 * TODO: Extend this to cascaded reflective shadow mapping
	*/

	_shadowMapBuffers [0]->BindForReading ();
}

void ReflectiveShadowMapDirectionalLightVolume::LockShader (int sceneLayers)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock the shader for animations
	*/

	if (sceneLayers & SceneLayer::ANIMATION) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_animationShaderName));
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_staticShaderName));
	}
}
