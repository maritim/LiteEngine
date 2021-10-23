#include "LPVCacheEmissiveRadianceRenderPass.h"

#include "LPVVolume.h"
#include "LPVGeometryVolume.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

LPVCacheEmissiveRadianceRenderPass::LPVCacheEmissiveRadianceRenderPass () :
	_vplCachesVolume (new VPLCachesVolume ())
{
	/*
	 * Nothing
	*/
}

LPVCacheEmissiveRadianceRenderPass::~LPVCacheEmissiveRadianceRenderPass ()
{
	/*
	 * Delete VPL caches volume
	*/

	delete _vplCachesVolume;
}

void LPVCacheEmissiveRadianceRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for static objects
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesCacheEmissiveRadianceVertex.glsl",
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesCacheEmissiveRadianceFragment.glsl",
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesCacheEmissiveRadianceGeometry.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* LPVCacheEmissiveRadianceRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	* Voxelization start
	*/

	StartVoxelization ();

	/*
	* Voxelization: voxelize geomtry
	*/

	GeometryVoxelizationPass (renderScene, settings, rvc);

	/*
	* Clear opengl state after voxelization
	*/

	EndVoxelization ();

	return rvc->Insert ("VPLCachesVolume", _vplCachesVolume);
}

void LPVCacheEmissiveRadianceRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

bool LPVCacheEmissiveRadianceRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute light propagation volumes emissive radiance injection render pass
	*/

	return !settings.lpv_emissive_voxelization && settings.lpv_emissive_cache;
}

void LPVCacheEmissiveRadianceRenderPass::StartVoxelization ()
{
	/*
	* Render to window but mask out all color.
	*/

	GL::Disable (GL_CULL_FACE);

	GL::ColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask (GL_FALSE);
}

void LPVCacheEmissiveRadianceRenderPass::GeometryVoxelizationPass (const RenderScene* renderScene,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	GL::Viewport (0, 0, 1, 1);

	/*
	* Render geometry
	*/

	for_each_type (RenderObject*, renderObject, *renderScene) {

		/*
		 * Check if it's active
		*/

		if (renderObject->IsActive () == false) {
			continue;
		}

		if (renderObject->GetRenderStage () != RenderStage::RENDER_STAGE_DEFERRED) {
			continue;
		}

		/*
		 * Check if the object is emissive
		*/

		auto modelView = renderObject->GetModelView ();

		bool isEmissive = false;

		for (auto& groupObject : *modelView) {

			// TODO: Fix this
			glm::vec3 emissiveColor = groupObject.materialView == nullptr ? glm::vec3 (0) :
				groupObject.materialView->emissiveColor;
			if (glm::dot (emissiveColor, emissiveColor) > 0.0f) {
				isEmissive = true;
				break;
			}
		}

		if (isEmissive == false) {
			continue;
		}

		/*
		 * Initialize VPL cache volume
		*/

		std::size_t triangleCount = modelView->GetPolygonsCount ();
		std::size_t vplCount = triangleCount * settings.lpv_emissive_vpls;

		auto vplCacheVolume = _vplCachesVolume->GetVPLCacheVolume (modelView->GetObjectBuffer ().VAO_INDEX);

		if (vplCacheVolume != nullptr && vplCacheVolume->GetSamplesCount () == vplCount) {
			continue;
		}

		// if (vplCacheVolume == nullptr || vplCacheVolume->GetSamplesCount () != vplCount) {
			vplCacheVolume = _vplCachesVolume->InsertVPLCacheVolume (modelView->GetObjectBuffer ().VAO_INDEX, vplCount);
		// }

		/*
		 * Lock voxelization shader for geomtry rendering
		*/

		LockShader ();

		/*
		 * Send vpl cache volume attributes to pipeline
		*/

		Pipeline::SendCustomAttributes (nullptr, vplCacheVolume->GetCustomAttributes ());

		/*
		 * Send custom attributes to pipeline
		*/

		Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (settings));

		/*
		 * Voxelize object
		*/

		renderObject->Draw ();
	}
}

void LPVCacheEmissiveRadianceRenderPass::EndVoxelization ()
{
	GL::MemoryBarrier(GL_ALL_BARRIER_BITS);

	/*
	* Clear settings
	*/

	GL::ColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask (GL_TRUE);

	/*
	* Unlock voxelization shader
	*/

	Pipeline::UnlockShader ();
}

void LPVCacheEmissiveRadianceRenderPass::LockShader ()
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock the shader
	*/

	Pipeline::LockShader (_shaderView);
}

std::vector<PipelineAttribute> LPVCacheEmissiveRadianceRenderPass::GetCustomAttributes (const RenderSettings& settings)
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute vplsCount;

	vplsCount.type = PipelineAttribute::AttrType::ATTR_1I;

	vplsCount.name = "vplsCount";

	vplsCount.value.x = settings.lpv_emissive_vpls;

	attributes.push_back (vplsCount);

	return attributes;
}
