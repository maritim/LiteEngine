#include "TemporalFilterRenderPass.h"

#include "RenderPasses/GBuffer.h"

#include "Renderer/Pipeline.h"

TemporalFilterRenderPass::TemporalFilterRenderPass () :
	_postProcessMapVolume2 (nullptr),
	_frameCount (0)
{

}

void TemporalFilterRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post process render pass
	*/

	PostProcessRenderPass::Init (settings);

	/*
	 * Initialize last post process map volume
	*/

	_postProcessMapVolume2 = CreatePostProcessVolume (settings);
}

RenderVolumeCollection* TemporalFilterRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	_frameCount = (_frameCount + 1) & 1;

	/*
	 * Update temporal anti-aliasing map volume
	*/

	UpdateLastPostProcessMapVolume (settings);

	/*
	 * Update settings
	*/

	UpdatePostProcessSettings (settings);

	/*
	 * Start screen space ambient occlusion generation pass
	*/

	StartPostProcessPass ();

	/*
	 * Screen space ambient occlusion generation pass
	*/

	PostProcessPass (renderScene, camera, settings, rvc);

	/*
	 * End screen space ambient occlusion generation pass
	*/

	EndPostProcessPass ();

	/*
	 * Keep current camera view projection matrix
	*/

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();
	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);

	_lastViewProjectionMatrix = projectionMatrix * viewMatrix;

	return rvc->Insert (GetPostProcessVolumeName (), GetCurrentPostProcessMapVolume ());
}

void TemporalFilterRenderPass::Clear ()
{
	/*
	 * Clear last post processing volume
	*/

	delete _postProcessMapVolume2;

	/*
	 * Clear post process render pass
	*/

	PostProcessRenderPass::Clear ();
}

void TemporalFilterRenderPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	GetCurrentPostProcessMapVolume ()->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);
}

std::string TemporalFilterRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 TemporalFilterRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height));
}

FramebufferRenderVolume* TemporalFilterRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	Resource<Texture> texture = Resource<Texture> (new Texture ("postProcessMap"));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	return new FramebufferRenderVolume (framebuffer);
}

std::vector<PipelineAttribute> TemporalFilterRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	auto gBuffer = (GBuffer*) rvc->GetRenderVolume ("GBuffer");

	PipelineAttribute temporalFilterMap;
	PipelineAttribute frustumJitter;
	PipelineAttribute reprojectionMatrix;

	temporalFilterMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	frustumJitter.type = PipelineAttribute::AttrType::ATTR_2F;
	reprojectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	temporalFilterMap.name = "temporalFilterMap";
	frustumJitter.name = "frustumJitter";
	reprojectionMatrix.name = "reprojectionMatrix";

	temporalFilterMap.value.x = GetLastPostProcessMapVolume ()->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex ();
	frustumJitter.value = glm::vec3 (gBuffer->GetFrustumJitter (), 0.0f);

	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	glm::mat4 reprojectionMat = screenMatrix * _lastViewProjectionMatrix * glm::inverse (viewMatrix);

	reprojectionMatrix.matrix = reprojectionMat;

	attributes.push_back (temporalFilterMap);
	attributes.push_back (frustumJitter);
	attributes.push_back (reprojectionMatrix);

	return attributes;
}

void TemporalFilterRenderPass::UpdateLastPostProcessMapVolume (const RenderSettings& settings)
{
	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	auto framebufferSize = _postProcessMapVolume2->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if (framebufferSize.width != (std::size_t) volumeResolution.x ||
		framebufferSize.height != (std::size_t) volumeResolution.y) {

		/*
		 * Clear temporal filtering map volume
		*/

		delete _postProcessMapVolume2;

		/*
		 * Initialize temporal filtering map volume
		*/

		_postProcessMapVolume2 = CreatePostProcessVolume (settings);
	}
}

FramebufferRenderVolume* TemporalFilterRenderPass::GetCurrentPostProcessMapVolume () const
{
	if (_frameCount == 0) {
		return _postProcessMapVolume;
	}

	if (_frameCount == 1) {
		return _postProcessMapVolume2;
	}

	return nullptr;
}

FramebufferRenderVolume* TemporalFilterRenderPass::GetLastPostProcessMapVolume () const
{
	if (_frameCount == 0) {
		return _postProcessMapVolume2;
	}

	if (_frameCount == 1) {
		return _postProcessMapVolume;
	}

	return nullptr;
}
