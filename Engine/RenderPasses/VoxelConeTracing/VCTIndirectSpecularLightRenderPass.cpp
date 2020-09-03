#include "VCTIndirectSpecularLightRenderPass.h"

#include "RenderPasses/IndirectSpecularLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/VCTStatisticsObject.h"

bool VCTIndirectSpecularLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string VCTIndirectSpecularLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingIndirectSpecularFragment.glsl";
}

std::string VCTIndirectSpecularLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectSpecularMap";
}

glm::ivec2 VCTIndirectSpecularLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("VCTStatisticsObject");
		VCTStatisticsObject* vctStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new VCTStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("VCTStatisticsObject", stat);
		}

		vctStatisticsObject = dynamic_cast<VCTStatisticsObject*> (stat);

		vctStatisticsObject->vctIndirectSpecularMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* VCTIndirectSpecularLightRenderPass::CreatePostProcessVolume () const
{
	IndirectSpecularLightMapVolume* indirectSpecularLightMapVolume = new IndirectSpecularLightMapVolume();

	return indirectSpecularLightMapVolume;
}

std::vector<PipelineAttribute> VCTIndirectSpecularLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute vctIndirectIntensity;
	PipelineAttribute specularConeDistance;
	PipelineAttribute originBias;

	vctIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	specularConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	originBias.type = PipelineAttribute::AttrType::ATTR_1F;

	vctIndirectIntensity.name = "vctIndirectIntensity";
	specularConeDistance.name = "specularConeDistance";
	originBias.name = "originBias";

	vctIndirectIntensity.value.x = settings.vct_indirect_specular_intensity;
	specularConeDistance.value.x = settings.vct_specular_cone_distance;
	originBias.value.x = settings.vct_origin_bias;

	attributes.push_back (vctIndirectIntensity);
	attributes.push_back (specularConeDistance);
	attributes.push_back (originBias);

	return attributes;
}
