#include "ShadowMapDirectionalLightVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Managers/ShaderManager.h"
#include "Systems/Window/Window.h"
#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

ShadowMapDirectionalLightVolume::ShadowMapDirectionalLightVolume () :
	_staticShaderName ("STATIC_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_SHADOW_MAP"),
	_shadowMapIndices (nullptr),
	_shadowMapResolutions (nullptr),
	_frameBufferIndex (0)
{
	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertex.glsl",
		"Assets/Shaders/ShadowMap/shadowMapFragment.glsl");

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertexAnimation.glsl",
		"Assets/Shaders/ShadowMap/shadowMapFragment.glsl");
}

ShadowMapDirectionalLightVolume::~ShadowMapDirectionalLightVolume ()
{
	/*
	 * Bind FBO
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, _frameBufferIndex);

	/*
	 * Detach current depth buffer from frame buffer object
	*/

	GL::FramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	/*
	 * Delete depth buffer textures
	*/

	GL::DeleteTextures (_cascadedLevels, _shadowMapIndices);

	/*
	 * Delete frame buffer
	*/

	GL::DeleteFramebuffers (1, &_frameBufferIndex);

	/*
	 * Restore default FBO
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	/*
	 * Delete cascaded resolutions
	*/

	delete [] _shadowMapResolutions;
}

bool ShadowMapDirectionalLightVolume::Init (std::size_t cascadedLevels)
{
	/*
	 * Initialize cascaded levels
	*/

	_cascadedLevels = cascadedLevels;

	/*
	 * Allocate memory for cascaded levels
	*/

	_shadowMapIndices = new GLuint [_cascadedLevels];
	_shadowMapResolutions = new std::pair<GLuint, GLuint> [_cascadedLevels];

	/*
	* Create Frame Buffer
	*/

	GL::GenFramebuffers(1, &_frameBufferIndex);

	/*
	 * Create shadow map cascaded textures 
	*/

	GL::GenTextures (_cascadedLevels, _shadowMapIndices);

	for (std::size_t index = 0; index < _cascadedLevels; index++) {
		GL::BindTexture(GL_TEXTURE_2D, _shadowMapIndices [index]);

		_shadowMapResolutions [index].first = SHADOW_MAP_MAX_RESOLUTION_WIDTH;
		_shadowMapResolutions [index].second = SHADOW_MAP_MAX_RESOLUTION_HEIGHT;

		GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
			_shadowMapResolutions [index].first, _shadowMapResolutions [index].second,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		GL::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	/*
	 * Attach depth buffer to frame buffer
	*/

	GL::BindFramebuffer(GL_FRAMEBUFFER, _frameBufferIndex);
	GL::FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMapIndices [0], 0);

	/*
	 * Disable writes to the color buffer
	*/

	GL::DrawBuffer(GL_NONE);
	GL::ReadBuffer(GL_NONE);

	/*
	 * Check that framebuffer is ok
	*/

	if (GL::CheckFramebufferStatus (GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Console::LogError ("Shadow Map Frame Buffer is not complete!");
		return false;
	}

	/*
	 * Restore default FBO
	*/

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void ShadowMapDirectionalLightVolume::BindForShadowMapCatch (std::size_t cascadedLevel)
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
	 * Bind frame buffer
	*/
	
	GL::BindFramebuffer(GL_FRAMEBUFFER, _frameBufferIndex);

	/*
	 * Set depth component based on cascade level
	*/

	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMapIndices [cascadedLevel], 0);

	/*
	 * Clear depth buffer
	*/

	GL::Clear (GL_DEPTH_BUFFER_BIT);
}

void ShadowMapDirectionalLightVolume::EndDrawing ()
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

void ShadowMapDirectionalLightVolume::BindForReading ()
{
	/*
	 * Bind every depth buffer for reading
	*/

	for (std::size_t index = 0;index < _cascadedLevels;index ++) {
		GL::ActiveTexture (GL_TEXTURE4 + index);
		GL::BindTexture (GL_TEXTURE_2D, _shadowMapIndices [index]);
	}
}

void ShadowMapDirectionalLightVolume::LockShader (int sceneLayers)
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
