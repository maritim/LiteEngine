#include "VoxelizationRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Window/Window.h"
#include "Systems/Settings/SettingsManager.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

VoxelizationRenderPass::VoxelizationRenderPass () :
	_voxelVolumeSize (0),
	_continuousVoxelization (false),
	_staticShaderName ("VOXELIZATION_PASS_STATIC_SHADER"),
	_animationShaderName ("VOXELIZATION_PASS_ANIMATION_SHADER"),
	_voxelVolume (new VoxelVolume ()),
	_firstTime (true)
{

}

VoxelizationRenderPass::~VoxelizationRenderPass ()
{
	delete _voxelVolume;
}

void VoxelizationRenderPass::Init ()
{
	/*
	 * Initialize voxelization settings
	*/

	InitSettings ();

	/*
	 * Initialize voxel volume
	*/

	InitVoxelVolume ();

	/*
	 * Shader for static objects
	*/

	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");

	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/Voxelize/voxelizeAnimationVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");
}

RenderVolumeCollection* VoxelizationRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	if (_firstTime || _continuousVoxelization) {

		/*
		* Voxelization start
		*/

		StartVoxelization ();

		/*
		* Voxelization: voxelize geomtry
		*/

		GeometryVoxelizationPass (scene);

		/*
		* Clear opengl state after voxelization
		*/

		EndVoxelization ();

		/*
		 * Send back the collection with voxel volume attached
		*/

		_firstTime = false;
	}

	return rvc->Insert ("VoxelVolume", _voxelVolume);
}

void VoxelizationRenderPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update voxel volume size
	*/

	if (name == "vct_voxels_size") {
		_voxelVolumeSize = SettingsManager::Instance ()->GetValue<int> ("vct_voxels_size", _voxelVolumeSize);

		/*
		 * Clear voxel volume
		*/

		_voxelVolume->Clear ();

		/*
		 * Initialize voxel volume
		*/

		InitVoxelVolume ();
	}

	/*
	 * Update continuous voxelization
	*/

	if (name == "vct_continuous_voxelization") {
		_continuousVoxelization = SettingsManager::Instance ()->GetValue<bool> ("vct_continuous_voxelization", _continuousVoxelization);
	}
}

void VoxelizationRenderPass::Clear ()
{
	/*
	 * Clear voxel volume
	*/

	_voxelVolume->Clear ();

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void VoxelizationRenderPass::StartVoxelization ()
{
	/*
	 * Clear voxel volume
	*/

	_voxelVolume->ClearVoxels ();

	/*
	* Render to window but mask out all color.
	*/

	GL::Disable (GL_CULL_FACE);

	GL::ColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask (GL_FALSE);
	GL::Viewport (0, 0, _voxelVolumeSize, _voxelVolumeSize);
}

void VoxelizationRenderPass::GeometryVoxelizationPass (const Scene* scene)
{
	/*
	* Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (scene);

	/*
	* Bind voxel volume to geometry render pass
	*/

	_voxelVolume->BindForWriting ();

	/*
	* Render geometry
	*/

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		 * Lock voxelization shader for geomtry rendering
		*/

		LockShader (sceneObject->GetLayers ());

		/*
		 * Send voxel volume attributes to pipeline
		*/

		Pipeline::SendCustomAttributes ("", _voxelVolume->GetCustomAttributes ());

		/*
		 * Voxelize object
		*/

		sceneObject->GetRenderer ()->Draw ();
	}
}

void VoxelizationRenderPass::EndVoxelization ()
{
	// GL::MemoryBarrier(GL_ALL_BARRIER_BITS);

	/*
	* Clear settings
	*/

	GL::Viewport (0, 0, Window::GetWidth (), Window::GetHeight ());
	GL::ColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask (GL_TRUE);

	GL::Enable (GL_CULL_FACE);

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
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_animationShaderName));
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_staticShaderName));
	}
}

void VoxelizationRenderPass::UpdateVoxelVolumeBoundingBox (const Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox ();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation ();

	glm::vec3 minVertex = volume->minVertex;
	glm::vec3 maxVertex = volume->maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void VoxelizationRenderPass::InitSettings ()
{
	/*
	 * Initialize voxel volume size
	*/

	_voxelVolumeSize = SettingsManager::Instance ()->GetValue<int> ("vct_voxels_size", _voxelVolumeSize);

	/*
	 * Initialize continuous voxelization
	*/

	_continuousVoxelization = SettingsManager::Instance ()->GetValue<bool> ("vct_continuous_voxelization", _continuousVoxelization);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Attach ("vct_continuous_voxelization", this);
}

void VoxelizationRenderPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Detach ("vct_continuous_voxelization", this);
}

void VoxelizationRenderPass::InitVoxelVolume ()
{
	if (!_voxelVolume->Init (_voxelVolumeSize)) {
		Console::LogError (std::string () +
			"Voxel texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (VOXEL_TEXTURE_NOT_INIT);
	}
}
