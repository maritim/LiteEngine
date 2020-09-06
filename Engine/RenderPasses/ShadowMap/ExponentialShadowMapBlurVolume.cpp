#include "ExponentialShadowMapBlurVolume.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

bool ExponentialShadowMapBlurVolume::Init (std::size_t bufferWidth, std::size_t bufferHeight)
{
	// /*
	//  * Keep resolution
	// */

	// _resolution = glm::ivec2 (bufferWidth, bufferHeight);

	// /*
	//  * Create framebuffer
	// */

	// GL::GenFramebuffers (1, &_fbo);
	// GL::BindFramebuffer (GL_FRAMEBUFFER, _fbo);

	// /*
	//  * Create the 2D color buffer texture
	// */

	// GL::GenTextures (1, &_colorBuffer);
	// GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);

	// GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	// GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	// GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// GL::TexImage2D (GL_TEXTURE_2D, 0, GL_R32F, _resolution.x, _resolution.y, 0, GL_RED, GL_FLOAT, 0);
		
	// GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);

	// /*
	//  * Get framebuffer status
	// */

	// GLenum status = GL::CheckFramebufferStatus (GL_FRAMEBUFFER);

	// /*
	//  * Unbind framebuffer
	// */

	// GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	// /*
	//  * Check that framebuffer was successfully initialized
	// */

	// if (status != GL_FRAMEBUFFER_COMPLETE) {
	// 	Console::LogError ("Framebuffer status error: " + status);
	// 	return false;
	// }

	return true;
}

const std::vector<PipelineAttribute>& ExponentialShadowMapBlurVolume::GetCustomAttributes () const
{
	// std::vector<PipelineAttribute> attributes;

	// PipelineAttribute postProcessMap;
	// PipelineAttribute blurMapResolution;

	// postProcessMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	// blurMapResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	// postProcessMap.name = "blurMap";
	// blurMapResolution.name = "blurMapResolution";

	// postProcessMap.value.x = _colorBuffer;
	// blurMapResolution.value = glm::vec3 (_resolution, 0);

	// attributes.push_back (postProcessMap);
	// attributes.push_back (blurMapResolution);

	// return attributes;

	return _attributes;
}
