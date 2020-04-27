#include "ResultFrameBufferGenerationRenderPass.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

ResultFrameBufferGenerationRenderPass::ResultFrameBufferGenerationRenderPass () :
	_frameBuffer (new ResultFrameBuffer2DVolume ())
{

}

ResultFrameBufferGenerationRenderPass::~ResultFrameBufferGenerationRenderPass ()
{
	delete _frameBuffer;
}

void ResultFrameBufferGenerationRenderPass::Init (const RenderSettings& settings)
{
	InitVolume (settings);
}

RenderVolumeCollection* ResultFrameBufferGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateVolume (settings);

	/*
	 * Bind light accumulation volume for writting
	*/

	_frameBuffer->BindForWriting ();

	/*
	 * Clear light accumulation buffer
	*/

	GL::DepthMask (GL_TRUE);
	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return rvc->Insert ("ResultFrameBuffer2DVolume", _frameBuffer);
}

bool ResultFrameBufferGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute idle container render sub pass
	*/

	return true;
}

void ResultFrameBufferGenerationRenderPass::Clear ()
{
	/*
	 * Clear post process map volume
	*/

	_frameBuffer->Clear ();
}

void ResultFrameBufferGenerationRenderPass::UpdateVolume (const RenderSettings& settings)
{
	Framebuffer framebuffer = settings.framebuffer;

	glm::ivec2 size = _frameBuffer->GetSize ();

	if ((std::size_t) size.x != framebuffer.width || (std::size_t) size.y != framebuffer.height) {

		/*
		 * Clear post process volume
		*/

		_frameBuffer->Clear ();

		/*
		 * Initialize post process volume
		*/

		InitVolume (settings);
	}
}

void ResultFrameBufferGenerationRenderPass::InitVolume (const RenderSettings& settings)
{
	if (!_frameBuffer->Init (glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height))) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (RESULT_FRAMEBUFFER_FBO_NOT_INIT);
	}
}