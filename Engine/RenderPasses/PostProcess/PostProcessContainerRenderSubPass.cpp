#include "PostProcessContainerRenderSubPass.h"

#include "Renderer/Pipeline.h"
#include "Managers/ShaderManager.h"

#include "Systems/Window/Window.h"

#include "Core/Console/Console.h"

PostProcessContainerRenderSubPass::PostProcessContainerRenderSubPass () :
	_postProcessMapVolume (nullptr)
{

}

PostProcessContainerRenderSubPass::~PostProcessContainerRenderSubPass ()
{
	delete _postProcessMapVolume;
}

void PostProcessContainerRenderSubPass::Init ()
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

	if (!_postProcessMapVolume->Init (Window::GetWidth (), Window::GetHeight ())) {
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

RenderVolumeCollection* PostProcessContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
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

	PostProcessPass (scene, camera, rvc);

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

void PostProcessContainerRenderSubPass::PostProcessPass (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
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

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (rvc));

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

std::vector<PipelineAttribute> PostProcessContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
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
