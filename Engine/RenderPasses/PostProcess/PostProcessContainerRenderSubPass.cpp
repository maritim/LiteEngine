#include "PostProcessContainerRenderSubPass.h"

#include "Renderer/Pipeline.h"
#include "Managers/ShaderManager.h"

#include "Core/Console/Console.h"

PostProcessContainerRenderSubPass::PostProcessContainerRenderSubPass () :
	_postProcessMapVolume (nullptr)
{

}

PostProcessContainerRenderSubPass::~PostProcessContainerRenderSubPass ()
{
	delete _postProcessMapVolume;
}

void PostProcessContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	ShaderManager::Instance ()->AddShader (GetPostProcessFragmentShaderPath (),
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		GetPostProcessFragmentShaderPath ());

	/*
	 * Initialize post processing volume
	*/

	_postProcessMapVolume = CreatePostProcessVolume ();

	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	if (!_postProcessMapVolume->Init (volumeResolution)) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}

void PostProcessContainerRenderSubPass::Clear ()
{
	/*
	 * Clear post processing volume
	*/

	_postProcessMapVolume->Clear ();
}

RenderVolumeCollection* PostProcessContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update settings
	*/

	UpdatePostProcessSettings (settings);

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

	PostProcessPass (scene, camera, settings, rvc);

	/*
	 * End screen space ambient occlusion generation pass
	*/

	EndPostProcessPass ();

	return rvc->Insert (GetPostProcessVolumeName (), _postProcessMapVolume);
}

void PostProcessContainerRenderSubPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	_postProcessMapVolume->BindForWriting ();
}

void PostProcessContainerRenderSubPass::PostProcessPass (const Scene* scene, const Camera* camera,
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

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (GetPostProcessFragmentShaderPath ()));

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

void PostProcessContainerRenderSubPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

void PostProcessContainerRenderSubPass::UpdatePostProcessSettings (const RenderSettings& settings)
{
	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	glm::ivec2 fbSize = _postProcessMapVolume->GetSize ();

	if (volumeResolution != fbSize) {

		/*
		 * Update post process map volume resolution
		*/

		ReinitPostProcessVolume (settings);
	}
}

std::vector<PipelineAttribute> PostProcessContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach all volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	for (RenderVolumeI* renderVolume : *rvc) {
		std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	return attributes;
}

void PostProcessContainerRenderSubPass::ReinitPostProcessVolume (const RenderSettings& settings)
{
	/*
	 * Clear current post process volume
	*/

	_postProcessMapVolume->Clear ();

	/*
	 * Initialize post process volume
	*/

	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	if (!_postProcessMapVolume->Init (volumeResolution)) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}