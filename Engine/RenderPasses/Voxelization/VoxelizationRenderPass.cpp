#include "VoxelizationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

bool VoxelizationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always allow voxelization pass
	*/

	return true;
}

void VoxelizationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for static objects
	*/

	Resource<Shader> staticShader = Resources::LoadShader ({
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl"
	});

	_staticShaderView = RenderSystem::LoadShader (staticShader);

	/*
	 * Shader for animated objects
	*/

	Resource<Shader> animationShader = Resources::LoadShader ({
		"Assets/Shaders/Voxelize/voxelizeAnimationVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl"
	});

	_animationShaderView = RenderSystem::LoadShader (animationShader);
}

RenderVolumeCollection* VoxelizationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	/*
	* Voxelization start
	*/

	StartVoxelization (voxelVolume);

	/*
	* Voxelization: voxelize geomtry
	*/

	GeometryVoxelizationPass (renderScene, settings, voxelVolume);

	/*
	* Clear opengl state after voxelization
	*/

	EndVoxelization ();

	/*
	 * Send back the collection with voxel volume attached
	*/

	return rvc;
}

void VoxelizationRenderPass::Clear ()
{

}

void VoxelizationRenderPass::StartVoxelization (VoxelVolume* voxelVolume)
{
	/*
	 * Clear voxel volume
	*/

	voxelVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor(0, 0, 0, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT);

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	/*
	* Render to window but mask out all color.
	*/

	GL::Disable (GL_CULL_FACE);

	GL::ColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask (GL_FALSE);
}

void VoxelizationRenderPass::GeometryVoxelizationPass (const RenderScene* renderScene,
	const RenderSettings& settings, VoxelVolume* voxelVolume)
{
	/*
	 * Set viewport
	*/

	GL::Viewport (0, 0, settings.vct_voxels_size, settings.vct_voxels_size);

	/*
	* Bind voxel volume to geometry render pass
	*/

	unsigned int voxelTextureID = voxelVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex ();
	GL::BindImageTexture (0, voxelTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

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
		 * Lock voxelization shader for geomtry rendering
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Send voxel volume attributes to pipeline
		*/

		Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (voxelVolume));

		/*
		 * Voxelize object
		*/

		renderObject->Draw ();
	}
}

void VoxelizationRenderPass::EndVoxelization ()
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

void VoxelizationRenderPass::LockShader (int sceneLayers)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock the shader for animations
	*/

	if (sceneLayers & SceneLayer::ANIMATION) {
		Pipeline::LockShader (_animationShaderView);
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (_staticShaderView);
	}
}

std::vector<PipelineAttribute> VoxelizationRenderPass::GetCustomAttributes (VoxelVolume* voxelVolume)
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSizeAttribute;
	PipelineAttribute volumeMipmapLevels;

	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSizeAttribute.type = PipelineAttribute::AttrType::ATTR_3I;
	volumeMipmapLevels.type = PipelineAttribute::AttrType::ATTR_1I;

	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSizeAttribute.name = "volumeSize";
	volumeMipmapLevels.name = "volumeMipmapLevels";

	minVertex.value = voxelVolume->GetMinVertex ();
	maxVertex.value = voxelVolume->GetMaxVertex ();
	volumeSizeAttribute.value = glm::vec3 ((float) voxelVolume->GetFramebuffer ()->GetTexture (0)->GetSize ().width);
	volumeMipmapLevels.value.x = voxelVolume->GetMipmapLevels ();

	attributes.push_back (minVertex);
	attributes.push_back (maxVertex);
	attributes.push_back (volumeSizeAttribute);
	attributes.push_back (volumeMipmapLevels);

	return attributes;
}
