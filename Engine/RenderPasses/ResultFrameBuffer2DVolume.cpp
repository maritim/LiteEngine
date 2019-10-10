#include "ResultFrameBuffer2DVolume.h"

#include "Wrappers/OpenGL/GL.h"

ResultFrameBuffer2DVolume::ResultFrameBuffer2DVolume () :
	FrameBuffer2DVolume (),
	_depthBuffer (0)
{

}

void ResultFrameBuffer2DVolume::AttachDepthBuffer (unsigned int depthBuffer)
{
	GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	GL::BindTexture (GL_TEXTURE_2D, depthBuffer);

	GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
}

void ResultFrameBuffer2DVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
}

void ResultFrameBuffer2DVolume::BindForWriting ()
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

std::vector<PipelineAttribute> ResultFrameBuffer2DVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightAccumulationMap;

	lightAccumulationMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	lightAccumulationMap.name = "lightAccumulationMap";

	lightAccumulationMap.value.x = _colorBuffer;

	attributes.push_back (lightAccumulationMap);

	return attributes;
}

unsigned int ResultFrameBuffer2DVolume::GetDepthBuffer () const
{
	return _depthBuffer;
}
