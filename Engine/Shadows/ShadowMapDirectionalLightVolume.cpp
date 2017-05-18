#include "ShadowMapDirectionalLightVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Managers/ShaderManager.h"
#include "Systems/Window/Window.h"
#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

#define SHADOW_MAP_RESOLUTION_WIDTH 2048		
#define SHADOW_MAP_RESOLUTION_HEIGHT 2048

ShadowMapDirectionalLightVolume::ShadowMapDirectionalLightVolume () :
	_shaderName ("SHADOW_MAP"),
	_shadowMapIndex (0),
	_frameBufferIndex (0)
{
	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertex.glsl",
		"Assets/Shaders/ShadowMap/shadowMapFragment.glsl");

	Init ();
}

ShadowMapDirectionalLightVolume::~ShadowMapDirectionalLightVolume ()
{

}

bool ShadowMapDirectionalLightVolume::Init ()
{
	/*
	* Create Frame Buffer
	*/

	GL::GenFramebuffers(1, &_frameBufferIndex);
	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferIndex);

	/*
	 * Create shadow map texture 
	*/
	GL::GenTextures(1, &_shadowMapIndex);
	GL::BindTexture(GL_TEXTURE_2D, _shadowMapIndex);
	GL::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_MAP_RESOLUTION_WIDTH, SHADOW_MAP_RESOLUTION_HEIGHT, 
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GL::TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	/*
	 * Attach depth buffer to frame buffer
	*/

	GL::BindFramebuffer(GL_FRAMEBUFFER, _frameBufferIndex);
	GL::FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMapIndex, 0);
	GL::DrawBuffer(GL_NONE);
	GL::ReadBuffer(GL_NONE);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);

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

void ShadowMapDirectionalLightVolume::BindForShadowMapCatch ()
{
	GL::Viewport (0, 0, SHADOW_MAP_RESOLUTION_WIDTH, SHADOW_MAP_RESOLUTION_HEIGHT);
	GL::BindFramebuffer(GL_FRAMEBUFFER, _frameBufferIndex);

	GL::Clear(GL_DEPTH_BUFFER_BIT);

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
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

void ShadowMapDirectionalLightVolume::BindForLightPass ()
{
	GL::ActiveTexture (GL_TEXTURE4);

	GL::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	GL::BindTexture (GL_TEXTURE_2D, _shadowMapIndex);
}