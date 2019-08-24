#include "RSMCachingRenderPass.h"

#include "Renderer/Pipeline.h"
#include "Managers/ShaderManager.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

#include "Core/Console/Console.h"

RSMCachingRenderPass::RSMCachingRenderPass () :
	_shaderName ("REFLECTIVE_SHADOW_MAPPING_CACHING"),
	_rsmCacheVolume (nullptr)
{

}

RSMCachingRenderPass::~RSMCachingRenderPass ()
{
	delete _rsmCacheVolume;
}

void RSMCachingRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize reflective shadow mapping cache shader
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMappingCachingFragment.glsl");

	/*
	 * Initialize reflective shadow mapping cache volume
	*/

	_rsmCacheVolume =  new RSMCacheVolume ();

	if (!_rsmCacheVolume->Init (settings.rsm_resolution)) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (REFLECTIVE_SHADOW_MAPPING_CACHE_VOLUME_NOT_INIT);
	}
}

void RSMCachingRenderPass::Clear ()
{
	/*
	 * Clear reflective shadow mapping cache volume
	*/

	_rsmCacheVolume->Clear ();
}

bool RSMCachingRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if reflective shadow mapping caching is enabled
	*/

	return settings.rsm_caching;
}

RenderVolumeCollection* RSMCachingRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update settings
	*/

	UpdateRSMCachingSettings (settings);

	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

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

	return rvc->Insert ("RSMCacheVolume", _rsmCacheVolume);
}

void RSMCachingRenderPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	_rsmCacheVolume->BindForWriting ();
}

void RSMCachingRenderPass::PostProcessPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * No rendering target
	*/

	GL::ColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask (GL_FALSE);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Lock post-process shader
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));

	/*
	 * Update matrices
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);
	Pipeline::SetObjectTransform (Transform::Default ());

	Pipeline::UpdateMatrices (nullptr);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (settings, rvc));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void RSMCachingRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();

	GL::ColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask (GL_TRUE);
}

void RSMCachingRenderPass::UpdateRSMCachingSettings (const RenderSettings& settings)
{
	glm::ivec2 fbSize = _rsmCacheVolume->GetSize ();

	if (settings.rsm_resolution != fbSize) {

		/*
		 * Update post process map volume resolution
		*/

		ReinitRSMCacheVolume (settings);
	}

	StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
	RSMStatisticsObject* rsmStatisticsObject = nullptr;

	if (stat == nullptr) {
		stat = new RSMStatisticsObject ();
		StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
	}

	rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

	rsmStatisticsObject->rsmCacheMapID = _rsmCacheVolume->GetColorTextureID ();
}

std::vector<PipelineAttribute> RSMCachingRenderPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach all volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	for (RenderVolumeI* renderVolume : *rvc) {
		std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	/*
	 * Attach reflective shadow mapping caching attributes to pipeline
	*/

	PipelineAttribute rsmResolution;
	PipelineAttribute rsmCacheMap;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmCacheMap.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmResolution.name = "rsmResolution";
	rsmCacheMap.name = "rsmCacheMap";

	rsmResolution.value = glm::vec3 (settings.rsm_resolution, 0.0f);
	rsmCacheMap.value.x = 22;

	attributes.push_back (rsmResolution);

	return attributes;
}

void RSMCachingRenderPass::ReinitRSMCacheVolume (const RenderSettings& settings)
{
	/*
	 * Clear current reflective shadow mapping cache volume
	*/

	_rsmCacheVolume->Clear ();

	/*
	 * Initialize reflective shadow mapping cache volume
	*/

	if (!_rsmCacheVolume->Init (settings.rsm_resolution)) {
		Console::LogError (std::string () + "Reflective shadow mapping cache volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (REFLECTIVE_SHADOW_MAPPING_CACHE_VOLUME_NOT_INIT);
	}
}