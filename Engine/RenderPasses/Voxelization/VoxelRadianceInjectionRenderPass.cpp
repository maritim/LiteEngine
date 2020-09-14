#include "VoxelRadianceInjectionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void VoxelRadianceInjectionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/Voxelize/voxelRadianceInjectionFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void VoxelRadianceInjectionRenderPass::Clear ()
{

}

RenderVolumeCollection* VoxelRadianceInjectionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Start screen space ambient occlusion generation pass
	*/

	StartPostProcessPass (rvc);

	/*
	 * Screen space ambient occlusion generation pass
	*/

	PostProcessPass (renderScene, camera, settings, renderLightObject, rvc);

	/*
	 * End screen space ambient occlusion generation pass
	*/

	EndPostProcessPass ();

	return rvc;
}

bool VoxelRadianceInjectionRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void VoxelRadianceInjectionRenderPass::StartPostProcessPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	auto voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	unsigned int voxelTextureID = voxelVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex ();
	GL::BindImageTexture (0, voxelTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void VoxelRadianceInjectionRenderPass::PostProcessPass (const RenderScene* renderScene,
	const Camera* camera, const RenderSettings& settings,
	const RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();
	glm::ivec2 rsmSize = shadow.resolution;

	GL::Viewport (0, 0, rsmSize.x, rsmSize.y);

	/*
	 * No rendering target
	*/

	// GL::DrawBuffer (GL_NONE);

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Lock post-process shader
	*/

	Pipeline::LockShader (_shaderView);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("ReflectiveShadowMapVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (renderLightObject, rvc));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void VoxelRadianceInjectionRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> VoxelRadianceInjectionRenderPass::GetCustomAttributes (const RenderLightObject* renderLightObject,
	RenderVolumeCollection* rvc) const
{
	auto voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmResolution;
	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSizeAttribute;
	PipelineAttribute volumeMipmapLevels;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSizeAttribute.type = PipelineAttribute::AttrType::ATTR_3I;
	volumeMipmapLevels.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmResolution.name = "rsmResolution";
	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSizeAttribute.name = "volumeSize";
	volumeMipmapLevels.name = "volumeMipmapLevels";

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	rsmResolution.value = glm::vec3 (shadow.resolution, 0.0f);
	minVertex.value = voxelVolume->GetMinVertex ();
	maxVertex.value = voxelVolume->GetMaxVertex ();
	volumeSizeAttribute.value = glm::vec3 ((float) voxelVolume->GetFramebuffer ()->GetTexture (0)->GetSize ().width);
	volumeMipmapLevels.value.x = voxelVolume->GetFramebuffer ()->GetTextureCount ();

	attributes.push_back (rsmResolution);
	attributes.push_back (minVertex);
	attributes.push_back (maxVertex);
	attributes.push_back (volumeSizeAttribute);
	attributes.push_back (volumeMipmapLevels);

	return attributes;
}
