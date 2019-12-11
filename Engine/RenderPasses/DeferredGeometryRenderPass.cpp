#include "DeferredGeometryRenderPass.h"

#include <algorithm>

#include "ResultFrameBuffer2DVolume.h"

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Debug/Profiler/Profiler.h"
#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RenderStatisticsObject.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

DeferredGeometryRenderPass::DeferredGeometryRenderPass () :
	_shaderName ("DEFERRED_GEOMETRY"),
	_normalMapShaderName ("NORMAL_MAP_DEFERRED_GEOMETRY"),
	_lightMapShaderName ("LIGHT_MAP_DEFERRED_GEOMETRY"),
	_animationShaderName ("ANIMATION_DEFERRED_GEOMETRY"),
	_frameBuffer (new GBuffer ()),
	_haltonGenerator (2, 3)
{

}

DeferredGeometryRenderPass::~DeferredGeometryRenderPass ()
{
	delete _frameBuffer;
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
}

RenderVolumeCollection* DeferredGeometryRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volumes
	*/

	UpdateVolumes (settings, rvc);

	/*
	* Send Camera to Pipeline
	*/

	UpdateCamera (camera, settings);

	/*
	* Deferred Rendering: Prepare for rendering
	*/

	PrepareDrawing ();

	/*
	* Deferred Rendering: Geometry Pass
	*/

	GeometryPass (renderScene, camera, settings);

	/*
	 * End geometry drawing
	*/

	EndDrawing ();

	return rvc->Insert ("GBuffer", _frameBuffer);
}

bool DeferredGeometryRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute skybox render sub pass
	*/

	return true;
}

void DeferredGeometryRenderPass::Clear ()
{
	/*
	 * Clear GBuffer volume
	*/

	_frameBuffer->Clear ();
}

void DeferredGeometryRenderPass::PrepareDrawing ()
{
	/*
	 * Bind framebuffer for writting
	*/

	_frameBuffer->BindForWriting ();
}

void DeferredGeometryRenderPass::GeometryPass (const RenderScene* renderScene, const Camera* camera, const RenderSettings& settings)
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
	 * Disable Blending
	*/

	GL::Disable (GL_BLEND);

	/*
	* Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	// std::vector<Renderer*> renderers;

	FrustumVolume* frustum = camera->GetFrustumVolume ();

	std::size_t drawnVerticesCount = 0;
	std::size_t drawnPolygonsCount = 0;
	std::size_t drawnObjectsCount = 0;

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
		* Culling Check
		*/

		if (renderObject->GetCollider () != nullptr) {
			GeometricPrimitive* primitive = renderObject->GetCollider ()->GetGeometricPrimitive ();
			if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
				continue;
			}
		}

		drawnVerticesCount += renderObject->GetModelView ()->GetVerticesCount ();
		drawnPolygonsCount += renderObject->GetModelView ()->GetPolygonsCount ();
		drawnObjectsCount++;

		/*
		 * Lock shader according to object layers
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Draw object on geometry buffer
		*/

		renderObject->Draw ();
	}

	static RenderStatisticsObject* renderStatisticsObject = new RenderStatisticsObject ();
	renderStatisticsObject->DrawnVerticesCount = drawnVerticesCount;
	renderStatisticsObject->DrawnPolygonsCount = drawnPolygonsCount;
	renderStatisticsObject->DrawnObjectsCount = drawnObjectsCount;

	StatisticsManager::Instance ()->SetStatisticsObject ("RenderStatisticsObject", renderStatisticsObject);

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

void DeferredGeometryRenderPass::UpdateCamera (const Camera* camera, const RenderSettings& settings)
{
	if (settings.taa_enabled == true) {
		glm::vec2 jitter = _haltonGenerator.Next () * 2.0f - 1.0f;

		jitter /= glm::vec2 (settings.viewport.width, settings.viewport.height);

		_frameBuffer->SetFrustumJitter (jitter);

		glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();
		glm::mat4 jitteringMatrix = glm::translate (glm::mat4 (1.0f), glm::vec3 (jitter, 0.0f));
		projectionMatrix = jitteringMatrix * projectionMatrix;

		_frameBuffer->SetProjectionMatrix (projectionMatrix);
	}

	if (settings.taa_enabled == false) {
		_frameBuffer->SetFrustumJitter (glm::vec2 (0.0f));

		_frameBuffer->SetProjectionMatrix (camera->GetProjectionMatrix ());
	}

	// Create projection matrix
	Pipeline::CreateProjection (_frameBuffer->GetProjectionMatrix ());

	// Create View Matrix
	Pipeline::SendCamera (camera);
}

void DeferredGeometryRenderPass::UpdateVolumes (const RenderSettings& settings, RenderVolumeCollection* rvc)
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

	/*
	 * Update result framebuffer
	*/

	ResultFrameBuffer2DVolume* resultFrameBufferVolume = (ResultFrameBuffer2DVolume*) rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");

	if (resultFrameBufferVolume->GetDepthBuffer () != _frameBuffer->GetDepthBuffer ()) {
		resultFrameBufferVolume->AttachDepthBuffer (_frameBuffer->GetDepthBuffer ());
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
