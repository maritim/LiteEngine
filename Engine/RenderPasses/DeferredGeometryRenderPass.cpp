#include "DeferredGeometryRenderPass.h"

#include <algorithm>

#include "FramebufferRenderVolume.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Debug/Profiler/Profiler.h"
#include "Debug/Statistics/StatisticsManager.h"
#include "DeferredStatisticsObject.h"
#include "RenderStatisticsObject.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

DeferredGeometryRenderPass::DeferredGeometryRenderPass () :
	_framebuffer (nullptr),
	_translucencyFramebuffer (nullptr),
	_haltonGenerator (2, 3)
{

}

DeferredGeometryRenderPass::~DeferredGeometryRenderPass ()
{

}

void DeferredGeometryRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for not animated objects
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredVertex.glsl",
		"Assets/Shaders/deferredFragment.glsl",
		"Assets/Shaders/deferredGeometry.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);

	/*
	 * Shader for not animated normal mapped objects
	*/

	Resource<Shader> normalMapShader = Resources::LoadShader ({
		"Assets/Shaders/deferredNormalMapVertex.glsl",
		"Assets/Shaders/deferredNormalMapFragment.glsl",
		"Assets/Shaders/deferredNormalMapGeometry.glsl"
	});

	_normalMapShaderView = RenderSystem::LoadShader (normalMapShader);

	/*
	 * Shader for not animated light mapped objects
	*/

	Resource<Shader> lightMapShader = Resources::LoadShader ({
		"Assets/Shaders/deferredLightMapVertex.glsl",
		"Assets/Shaders/deferredLightMapFragment.glsl",
		"Assets/Shaders/deferredLightMapGeometry.glsl"
	});

	_lightMapShaderView = RenderSystem::LoadShader (lightMapShader);

	/*
	 * Shader for animations
	*/

	Resource<Shader> animationShader = Resources::LoadShader ({
		"Assets/Shaders/deferredVertexAnimation.glsl",
		"Assets/Shaders/deferredFragment.glsl",
		"Assets/Shaders/deferredGeometry.glsl"
	});

	_animationShaderView = RenderSystem::LoadShader (animationShader);

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

	PrepareDrawing (camera);

	/*
	* Deferred Rendering: Geometry Pass
	*/

	GeometryPass (renderScene, camera, settings);

	/*
	 * End geometry drawing
	*/

	EndDrawing ();

	/*
	 * Generate mipmaps
	*/

	GenerateMipmaps ();

	return rvc->Insert ("GBuffer", _framebuffer)
				->Insert ("TranslucencyGBuffer", _translucencyFramebuffer);
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

	delete _translucencyFramebuffer;
	delete _framebuffer;
}

void DeferredGeometryRenderPass::PrepareDrawing (const Camera* camera)
{
	GL::DepthMask (GL_TRUE);

	/*
	 * Clear framebuffer
	*/

	ClearGBuffer (_framebuffer, camera);

	/*
	 * Clear translucency framebuffer
	*/

	ClearGBuffer (_translucencyFramebuffer, camera);
}

void DeferredGeometryRenderPass::ClearGBuffer (GBuffer* framebuffer, const Camera* camera)
{
	/*
	 * Clear all buffers
	*/

	_framebuffer->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*
	 * Clear position buffer
	*/

	GL::DrawBuffer (GL_COLOR_ATTACHMENT0);

	GL::ClearColor (0, 0, -camera->GetZFar (), 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);
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

	auto frustum = camera->GetFrustumVolume ();

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

		auto& boundingBox = renderObject->GetBoundingBox ();
		if (!Intersection::Instance ()->CheckFrustumVsAABB (frustum, boundingBox)) {
			continue;
		}

		drawnVerticesCount += renderObject->GetModelView ()->GetVerticesCount ();
		drawnPolygonsCount += renderObject->GetModelView ()->GetPolygonsCount ();
		drawnObjectsCount++;

		/*
		* Deferred Rendering: Prepare for rendering
		*/

		BindFrameBuffer (renderObject->GetSceneLayers ());

		/*
		 * Lock shader according to object layers
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Draw object on geometry buffer
		*/

		renderObject->Draw ();
	}

	auto renderStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RenderStatisticsObject> ();

	renderStatisticsObject->DrawnVerticesCount = drawnVerticesCount;
	renderStatisticsObject->DrawnPolygonsCount = drawnPolygonsCount;
	renderStatisticsObject->DrawnObjectsCount = drawnObjectsCount;

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

void DeferredGeometryRenderPass::GenerateMipmaps ()
{
	/*
	 * Generate GBuffer mipmap
	*/

	for (std::size_t index = 0 ; index < 4 ; index++) {
		_framebuffer->GetFramebufferView ()->GetTextureView (index)->Activate (0);

		GL::GenerateMipmap (GL_TEXTURE_2D);
	}
}

void DeferredGeometryRenderPass::BindFrameBuffer (int sceneLayers)
{
	/*
	 * Bind generic framebuffer
	*/

	if (!(sceneLayers & SceneLayer::TRANSLUCENCY)) {
		_framebuffer->GetFramebufferView ()->Activate ();
	}

	/*
	 * Bind framebuffer for translucency
	*/

	if (sceneLayers & SceneLayer::TRANSLUCENCY) {
		_translucencyFramebuffer->GetFramebufferView ()->Activate ();
	}
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
		Pipeline::LockShader (_animationShaderView);
	}

	/*
	 * Lock the shader for not animated normal mapped objects
	*/

	if ((sceneLayers & SceneLayer::NORMAL_MAP) && (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC))) {
		Pipeline::LockShader (_normalMapShaderView);
	}

	/*
	 * Lock the shader for static light mapped objects
	*/

	if ((sceneLayers & SceneLayer::LIGHT_MAP) && (sceneLayers & SceneLayer::STATIC)) {
		Pipeline::LockShader (_lightMapShaderView);
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if ((sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) && !(sceneLayers & SceneLayer::NORMAL_MAP)) {
		Pipeline::LockShader (_shaderView);
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
		glm::vec2 jitter = _haltonGenerator.Next () - 0.5f;

		jitter /= glm::vec2 (settings.viewport.width, settings.viewport.height);

		_framebuffer->SetFrustumJitter (jitter);

		glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();
		glm::mat4 jitteringMatrix = glm::translate (glm::mat4 (1.0f), glm::vec3 (jitter, 0.0f));
		projectionMatrix = jitteringMatrix * projectionMatrix;

		_framebuffer->SetProjectionMatrix (projectionMatrix);
	}

	if (settings.taa_enabled == false) {
		_framebuffer->SetFrustumJitter (glm::vec2 (0.0f));

		_framebuffer->SetProjectionMatrix (camera->GetProjectionMatrix ());
	}

	// Create projection matrix
	Pipeline::CreateProjection (_framebuffer->GetProjectionMatrix ());

	// Create View Matrix
	Pipeline::SendCamera (camera);
}

void DeferredGeometryRenderPass::UpdateVolumes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	Resolution resolution = settings.resolution;

	auto framebufferSize = _framebuffer->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if (framebufferSize.width != resolution.width ||
		framebufferSize.height != resolution.height) {

		/*
		 * Clear framebuffer
		*/

		delete _framebuffer;
		delete _translucencyFramebuffer;

		/*
		 * Initialize framebuffer
		*/

		InitGBufferVolume (settings);
	}

	/*
	 * Update result framebuffer
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("ResultFramebufferRenderVolume");

	auto depthTextureView = _framebuffer->GetFramebufferView ()->GetDepthTextureView ();

	if (resultVolume->GetFramebufferView ()->GetDepthTextureView () != depthTextureView) {

		resultVolume->GetFramebufferView ()->Activate ();

		depthTextureView->Activate (0);

		GL::FramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTextureView->GetGPUIndex (), 0);

		/*
		 * Update framebuffer view
		*/

		resultVolume->GetFramebufferView ()->SetDepthTextureView (depthTextureView);
	}
}

void DeferredGeometryRenderPass::InitGBufferVolume (const RenderSettings& settings)
{
	/*
	 * Initialize GBuffer volume
	*/

	std::vector<Resource<Texture>> textures;
	Resource<Texture> depthTexture;

	std::vector<std::string> volumeNames = { "gPositionMap", "gNormalMap", "gDiffuseMap", "gSpecularMap", "gEmissiveMap" };

	for (auto volumeName : volumeNames) {
		Resource<Texture> texture = Resource<Texture> (new Texture (volumeName));

		texture->SetSize (Size (settings.resolution.width, settings.resolution.height));
		texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGBA32);
		texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGBA);
		texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
		texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
		texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST_MIPMAP_NEAREST);
		texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
		texture->SetAnisotropicFiltering (false);
		texture->SetBorderColor (Color (glm::vec4 (1.0)));

		textures.push_back (texture);
	}

	depthTexture = Resource<Texture> (new Texture ("gDepthMap"));

	depthTexture->SetSize (Size (settings.resolution.width, settings.resolution.height));
	depthTexture->SetMipmapGeneration (false);
	depthTexture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_DEPTH24_STENCIL8);
	depthTexture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_DEPTH_STENCIL);
	depthTexture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	depthTexture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
	depthTexture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	depthTexture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	depthTexture->SetAnisotropicFiltering (false);
	depthTexture->SetBorderColor (Color (glm::vec4 (1.0)));

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (textures, depthTexture));

	_framebuffer = new GBuffer (framebuffer);

	auto deferredStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <DeferredStatisticsObject> ();

	deferredStatisticsObject->geometryBuffer = _framebuffer;

	/*
	 * Initialize translucency gbuffer
	*/

	volumeNames = { "gTrPositionMap", "gTrNormalMap", "gTrDiffuseMap", "gTrSpecularMap", "gTrEmissiveMap" };

	for (std::size_t index = 0; index < textures.size (); index ++) {
		textures [index]->SetName (volumeNames [index]);
		textures [index]->SetMipmapGeneration (false);
		textures [index]->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	}

	depthTexture->SetName ("gTrDepthMap");

	framebuffer = Resource<Framebuffer> (new Framebuffer (textures, depthTexture));

	_translucencyFramebuffer = new GBuffer (framebuffer);

	deferredStatisticsObject->translucencyGeometryBuffer = _translucencyFramebuffer;
}
