#include "PostProcessRenderPass.h"

#include "RenderPasses/GBuffer.h"

#include "Renderer/Pipeline.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Core/Console/Console.h"

PostProcessRenderPass::PostProcessRenderPass () :
	_postProcessMapVolume (nullptr)
{

}

void PostProcessRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		GetPostProcessFragmentShaderPath ()
	});

	_shaderView = RenderSystem::LoadShader (shader);

	/*
	 * Create post processing volume
	*/

	_postProcessMapVolume = CreatePostProcessVolume (settings);
}

void PostProcessRenderPass::Clear ()
{
	/*
	 * Clear post processing volume
	*/

	delete _postProcessMapVolume;
}

RenderVolumeCollection* PostProcessRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
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

	return rvc->Insert (GetPostProcessVolumeName (), _postProcessMapVolume);
}

void PostProcessRenderPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	_postProcessMapVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);
}

void PostProcessRenderPass::PostProcessPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	GL::Viewport (0, 0, volumeResolution.x, volumeResolution.y);

	/*
	 * Enable color blending
	*/

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Lock post-process shader
	*/

	Pipeline::LockShader (_shaderView);

	/*
	 * Update matrices
	*/

	Pipeline::CreateProjection (((GBuffer*) rvc->GetRenderVolume ("GBuffer"))->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);
	Pipeline::SetObjectTransform (Transform::Default ());

	Pipeline::UpdateMatrices (nullptr);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (camera, settings, rvc));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void PostProcessRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

void PostProcessRenderPass::UpdatePostProcessSettings (const RenderSettings& settings)
{
	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	auto framebufferSize = _postProcessMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if ((std::size_t) volumeResolution.x != framebufferSize.width ||
		(std::size_t) volumeResolution.y != framebufferSize.height) {

		/*
		 * Clear current post process volume
		*/

		delete _postProcessMapVolume;

		/*
		 * Initialize post process volume
		*/

		_postProcessMapVolume = CreatePostProcessVolume (settings);
	}
}

std::vector<PipelineAttribute> PostProcessRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach all volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	for (RenderVolumeI* renderVolume : *rvc) {
		auto& volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	return attributes;
}
