#include "LightAccumulationVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

LightAccumulationVolume::LightAccumulationVolume () :
	FrameBuffer2DVolume (),
	_depthBuffer (0)
{

}

LightAccumulationVolume::~LightAccumulationVolume ()
{

}

bool LightAccumulationVolume::Init (const glm::ivec2& size, GBuffer* gBuffer)
{
	/*
	 * Initialize framebuffer
	*/

	if (!FrameBuffer2DVolume::Init (size)) {
		return false;
	}

	/*
	 * Initialize depth buffer from geometry buffer
	*/

	_depthBuffer = gBuffer->GetDepthBuffer ();

	/*
	 * Attach depth buffer texture
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	GL::BindTexture (GL_TEXTURE_2D, _depthBuffer);

	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);

	/*
	 * Get framebuffer status
	*/

	GLenum status = GL::CheckFramebufferStatus (GL_FRAMEBUFFER);

	/*
	 * Unbind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	/*
	 * Check that framebuffer was successfully initialized
	*/

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Console::LogError ("Framebuffer status error: " + status);
		return false;
	}

	return true;
}

void LightAccumulationVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE4);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

void LightAccumulationVolume::BindForWriting ()
{
	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	/*
	 * Bind final texture as drawing output
	*/

	GL::DrawBuffer (GL_COLOR_ATTACHMENT0);
}

std::vector<PipelineAttribute> LightAccumulationVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightAccumulationMap;

	lightAccumulationMap.type = PipelineAttribute::AttrType::ATTR_1I;

	lightAccumulationMap.name = "lightAccumulationMap";

	lightAccumulationMap.value.x = 4;

	attributes.push_back (lightAccumulationMap);

	return attributes;
}
