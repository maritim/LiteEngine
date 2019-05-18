#include "DeferredGeometryRenderPass.h"

#include <algorithm>

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Debug/Profiler/Profiler.h"
#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/DrawnObjectsCountStat.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

DeferredGeometryRenderPass::DeferredGeometryRenderPass () :
	_shaderName ("DEFERRED_GEOMETRY"),
	_normalMapShaderName ("NORMAL_MAP_DEFERRED_GEOMETRY"),
	_lightMapShaderName ("LIGHT_MAP_DEFERRED_GEOMETRY"),
	_animationShaderName ("ANIMATION_DEFERRED_GEOMETRY"),
	_frameBuffer (new GBuffer ()),
	_lightAccumulationVolume (new LightAccumulationVolume ())
{

}

DeferredGeometryRenderPass::~DeferredGeometryRenderPass ()
{
	delete _frameBuffer;
	delete _lightAccumulationVolume;
}

void DeferredGeometryRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for not animated objects
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredVertex.glsl",
		"Assets/Shaders/deferredFragment.glsl",
		"Assets/Shaders/deferredGeometry.glsl");

	/*
	 * Shader for not animated normal mapped objects
	*/

	ShaderManager::Instance ()->AddShader (_normalMapShaderName,
		"Assets/Shaders/deferredNormalMapVertex.glsl",
		"Assets/Shaders/deferredNormalMapFragment.glsl",
		"Assets/Shaders/deferredNormalMapGeometry.glsl");

	/*
	 * Shader for not animated light mapped objects
	*/

	ShaderManager::Instance ()->AddShader (_lightMapShaderName,
		"Assets/Shaders/deferredLightMapVertex.glsl",
		"Assets/Shaders/deferredLightMapFragment.glsl",
		"Assets/Shaders/deferredLightMapGeometry.glsl");

	/*
	 * Shader for animations
	*/

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/deferredVertexAnimation.glsl",
		"Assets/Shaders/deferredFragment.glsl",
		"Assets/Shaders/deferredGeometry.glsl");

	/*
	 * Initialize GBuffer volume
	*/

	InitGBufferVolume (settings);

	/*
	 * Initialize light accumulation volume
	*/

	InitLightAccumulationVolume (settings);
}

RenderVolumeCollection* DeferredGeometryRenderPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volumes
	*/

	UpdateVolumes (settings);

	/*
	* Send Camera to Pipeline
	*/

	UpdateCamera (camera);

	/*
	* Deferred Rendering: Prepare for rendering
	*/

	PrepareDrawing ();

	/*
	* Deferred Rendering: Geometry Pass
	*/

	GeometryPass (scene, camera, settings);

	/*
	 * End geometry drawing
	*/

	EndDrawing ();

	return rvc->Insert ("GBuffer", _frameBuffer)->Insert ("LightAccumulationVolume", _lightAccumulationVolume);
}

void DeferredGeometryRenderPass::Clear ()
{
	/*
	 * Clear GBuffer volume
	*/

	_frameBuffer->Clear ();

	/*
	 * Clear light accumulation volume
	*/

	_lightAccumulationVolume->Clear ();
}

void DeferredGeometryRenderPass::PrepareDrawing ()
{
	/*
	 * Bind light accumulation volume for writting
	*/

	_lightAccumulationVolume->BindForWriting ();

	/*
	 * Clear light accumulation buffer
	*/

	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	 * Bind framebuffer for writting
	*/

	_frameBuffer->BindForWriting ();
}

void DeferredGeometryRenderPass::GeometryPass (const Scene* scene, const Camera* camera, const RenderSettings& settings)
{
	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * Set Depth Buffer
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_TRUE);

	/*
	* Set Stencil Buffer to know where something is drawn in GBuffer
	* Reference: http://www.gamedev.net/topic/570610-deferred-lighting---skybox-objects-without-light-/
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	* Enable Back Face Culling
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_BACK);

	/*
	 * Enable Blending
	*/

	GL::Enable (GL_BLEND);

	/*
	* Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	std::vector<Renderer*> renderers;

	FrustumVolume* frustum = camera->GetFrustumVolume ();

	std::size_t drawnObjectsCount = 0;

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		* Culling Check
		*/

		if (sceneObject->GetCollider () == nullptr) {
			continue;
		}

		GeometricPrimitive* primitive = sceneObject->GetCollider ()->GetGeometricPrimitive ();
		if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
			continue;
		}

		drawnObjectsCount++;

		/*
		 * Lock shader according to object layers
		*/

		LockShader (sceneObject->GetLayers ());

		/*
		 * Draw object on geometry buffer
		*/

		sceneObject->GetRenderer ()->Draw ();
	}

	static DrawnObjectsCountStat* drawnObjectsCountStat = new DrawnObjectsCountStat ();
	drawnObjectsCountStat->SetDrawnObjectsCount (drawnObjectsCount);

	StatisticsManager::Instance ()->SetStatisticsObject ("DrawnObjectsCount", drawnObjectsCountStat);

	/*
	* Disable Stecil Test for further rendering
	*/

	GL::Disable (GL_STENCIL_TEST);
}

void DeferredGeometryRenderPass::EndDrawing ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}

void DeferredGeometryRenderPass::LockShader (int sceneLayers)
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
	 * Lock the shader for not animated normal mapped objects
	*/

	if ((sceneLayers & SceneLayer::NORMAL_MAP) && (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC))) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_normalMapShaderName));
	}

	/*
	 * Lock the shader for static light mapped objects
	*/

	if ((sceneLayers & SceneLayer::LIGHT_MAP) && (sceneLayers & SceneLayer::STATIC)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_lightMapShaderName));
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if ((sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) && !(sceneLayers & SceneLayer::NORMAL_MAP)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
	}
}

/*
* TODO: Move this part somewhere else because it belongs to another
* abstraction layer. This class only work with objects rendering, not
* pipeline's job
*/

void DeferredGeometryRenderPass::UpdateCamera (const Camera* camera)
{
	// Create Projection
	Pipeline::CreateProjection (camera);

	// Create View Matrix
	Pipeline::SendCamera (camera);
}

void DeferredGeometryRenderPass::UpdateVolumes (const RenderSettings& settings)
{
	Framebuffer framebuffer = settings.framebuffer;

	glm::ivec2 fbSize = _frameBuffer->GetSize ();

	if ((std::size_t) fbSize.x != framebuffer.width || (std::size_t) fbSize.y != framebuffer.height) {

		/*
		 * Clear framebuffer
		*/

		_frameBuffer->Clear ();

		/*
		 * Initialize framebuffer
		*/

		InitGBufferVolume (settings);
	}

	glm::ivec2 lightVolumeSize = _lightAccumulationVolume->GetSize ();

	if ((std::size_t) lightVolumeSize.x != framebuffer.width || (std::size_t) lightVolumeSize.y != framebuffer.height) {

		/*
		 * Clear light accumulation volume
		*/

		_lightAccumulationVolume->Clear ();

		/*
		 * Initialize light accumulation volume
		*/

		InitLightAccumulationVolume (settings);
	}
}

void DeferredGeometryRenderPass::InitGBufferVolume (const RenderSettings& settings)
{
	/*
	 * Initialize GBuffer volume
	*/

	if (!_frameBuffer->Init (glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height))) {
		Console::LogError (std::string () +
			"Geometry buffer for deferred rendering cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (GBUFFER_FBO_NOT_INIT);
	}
}

void DeferredGeometryRenderPass::InitLightAccumulationVolume (const RenderSettings& settings)
{
	/*
	 * Initialize light accumulation volume
	*/

	if (!_lightAccumulationVolume->Init (glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height), _frameBuffer)) {
		Console::LogError (std::string () +
			"Light accumulation buffer for deferred rendering cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_ACCUMULATION_FBO_NOT_INIT);
	}
}
