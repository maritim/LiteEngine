#include "TextureLUTRenderPass.h"

#include "Resources/Resources.h"

#include "Renderer/RenderSystem.h"

void TextureLUTRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post process render pass
	*/

	PostProcessRenderPass::Init (settings);

	/*
	 * Initialize texture LUT view
	*/

	InitTextureLUTView (settings);
}

bool TextureLUTRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if texture LUT is enabled
	*/

	return settings.lut_enabled;
}

std::string TextureLUTRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TextureLUT/textureLUTFragment.glsl";
}

std::string TextureLUTRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 TextureLUTRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* TextureLUTRenderPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> TextureLUTRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Update texture LUT view
	*/

	UpdateTextureLUTView (settings);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute lutTexture;
	PipelineAttribute lutIntensity;

	lutTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;
	lutIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lutTexture.name = "lutTexture";
	lutIntensity.name = "lutIntensity";

	lutTexture.value.x = _lutTextureView->GetGPUIndex ();
	lutIntensity.value.x = settings.lut_intensity;

	attributes.push_back (lutTexture);
	attributes.push_back (lutIntensity);

	return attributes;
}

void TextureLUTRenderPass::UpdateTextureLUTView (const RenderSettings& settings)
{
	if (_lutTexturePath != settings.lut_texture_path) {

		/*
		 * Update texture LUT path
		*/

		_lutTexturePath = settings.lut_texture_path;

		/*
		 * Initialize temporal anti-aliasing map volume
		*/

		InitTextureLUTView (settings);
	}
}

void TextureLUTRenderPass::InitTextureLUTView (const RenderSettings& settings)
{
	/*
	 * Initialize texture LUT view
	*/

	Resource<Texture> lutTexture = Resources::LoadTexture (settings.lut_texture_path);
	lutTexture->SetName (lutTexture->GetName () + "View");

	_lutTextureView = RenderSystem::LoadTextureLUT (lutTexture);
}
