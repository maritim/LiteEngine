#include "SSAONoiseGenerationRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSAOStatisticsObject.h"

#include "Core/Random/Random.h"

bool SSAONoiseGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

glm::ivec2 SSAONoiseGenerationRenderPass::GetNoiseMapVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.ssao_noise_size, settings.ssao_noise_size);
}

TextureRenderVolume* SSAONoiseGenerationRenderPass::CreateNoiseMapVolume (const RenderSettings& settings) const
{
	glm::ivec2 size = GetNoiseMapVolumeResolution (settings);

	/*
	 * Generate noise values
	*/

	std::vector<float> ssaoNoise;

	std::size_t pixelsCount = size.x * size.y;

	for (std::size_t pixelIndex = 0; pixelIndex < pixelsCount; pixelIndex ++) {

		ssaoNoise.push_back (Random::Instance ()->RangeF (-1.0f, 1.0f));
		ssaoNoise.push_back (Random::Instance ()->RangeF (-1.0f, 1.0f));
	}

	/*
	 * Create screen space ambient occlusion noise map
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("noiseMap"));

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RG32F);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RG);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetPixels ((const unsigned char*) ssaoNoise.data (), ssaoNoise.size () * sizeof (float));

	TextureRenderVolume* renderVolume = new TextureRenderVolume (texture);

	/*
	 * Update statistics
	*/

	auto ssaoStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSAOStatisticsObject> ();

	ssaoStatisticsObject->ssaoNoiseMapVolume = renderVolume;

	return renderVolume;
}
