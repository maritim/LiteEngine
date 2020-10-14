#include "RSMAccumulationRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Core/Console/Console.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RSMStatisticsObject.h"

RSMAccumulationRenderPass::RSMAccumulationRenderPass () :
	_rsmVolume (nullptr)
{

}

RenderVolumeCollection* RSMAccumulationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Update volumes
	*/

	UpdateRSMVolume (renderLightObject);

	/*
	* Start shadow map drawing process
	*/

	StartShadowMapPass ();

	/*
	* Calculate light camera for shadow map
	*/

	Camera* lightCamera = GetLightCamera (renderScene, renderLightObject);
	_rsmVolume->SetLightCamera (lightCamera);

	/*
	* Render geometry on shadow map
	*/

	ShadowMapGeometryPass (renderScene, lightCamera, settings, renderLightObject);

	/*
	* End shadow map drawing process
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ReflectiveShadowMapVolume", _rsmVolume, false);
}

bool RSMAccumulationRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Execute reflective shadow map accumulation sub pass only if light is casting shadows
	*/

	return renderLightObject->IsCastingShadows ();
}

void RSMAccumulationRenderPass::Clear ()
{
	delete _rsmVolume;
}

void RSMAccumulationRenderPass::StartShadowMapPass ()
{
	/*
	* Bind shadow map volume for writing
	*/

	_rsmVolume->GetFramebufferView ()->Activate ();
}

void RSMAccumulationRenderPass::ShadowMapGeometryPass (const RenderScene* renderScene, const Camera* lightCamera,
	const RenderSettings& settings, const RenderLightObject* renderLightObject)
{
	/*
	* Send light camera
	*/

	Pipeline::CreateProjection (lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (lightCamera);

	/*
	 * Clear framebuffer
	*/

	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*
	 * Set viewport
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	GL::Viewport (0, 0, shadow.resolution.x, shadow.resolution.y);

	/*
	* Shadow map is a depth test
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_TRUE);

	/*
	* Doesn't really matter
	*/

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	/*
	 * Enable back face culling
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_BACK);

	/*
	* Light camera
	*/

	auto frustum = lightCamera->GetFrustumVolume ();

	/*
	* Render scene entities to framebuffer at Deferred Rendering Stage
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
		* Culling Check
		*/

		auto& boundingBox = renderObject->GetBoundingBox ();
		if (!Intersection::Instance ()->CheckFrustumVsAABB (frustum, boundingBox)) {
			continue;
		}

		/*
		* Lock shader based on scene object layers
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Send custom attributes
		*/

		Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (renderLightObject));

		/*
		* Render object on shadow map
		*/

		renderObject->Draw ();
	}
}

void RSMAccumulationRenderPass::EndShadowMapPass ()
{
	Pipeline::UnlockShader ();
}

void RSMAccumulationRenderPass::InitRSMVolume (const RenderLightObject* renderLightObject)
{
	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	/*
	 * Create framebuffer
	*/

	std::vector<Resource<Texture>> textures;
	Resource<Texture> depthTexture;

	std::vector<std::string> volumeNames = { "rsmPositionMap", "rsmNormalMap", "rsmFluxMap" };

	for (auto volumeName : volumeNames) {
		Resource<Texture> texture = Resource<Texture> (new Texture (volumeName));

		texture->SetSize (Size (shadow.resolution.x, shadow.resolution.y));
		texture->SetMipmapGeneration (false);
		texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
		texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
		texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
		texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
		texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
		texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
		texture->SetAnisotropicFiltering (false);

		textures.push_back (texture);
	}

	depthTexture = Resource<Texture> (new Texture ("shadowMap"));

	depthTexture->SetSize (Size (shadow.resolution.x, shadow.resolution.y));
	depthTexture->SetMipmapGeneration (false);
	depthTexture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_DEPTH32);
	depthTexture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_DEPTH);
	depthTexture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	depthTexture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
	depthTexture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	depthTexture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	depthTexture->SetAnisotropicFiltering (false);
	depthTexture->SetBorderColor (Color (glm::vec4 (1.0)));

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (textures, depthTexture));

	_rsmVolume = new RSMVolume (framebuffer);

	_rsmVolume->SetShadowBias (shadow.bias);
}

void RSMAccumulationRenderPass::UpdateRSMVolume (const RenderLightObject* renderLightObject)
{
	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (_rsmVolume == nullptr ||
		_rsmVolume->GetFramebuffer ()->GetTexture (0)->GetSize ().width != (std::size_t) shadow.resolution.x ||
		_rsmVolume->GetFramebuffer ()->GetTexture (0)->GetSize ().height != (std::size_t) shadow.resolution.y) {

		/*
		 * Clear framebuffer
		*/

		delete _rsmVolume;

		/*
		 * Initialize framebuffer
		*/

		InitRSMVolume (renderLightObject);
	}

	/*
	 * Update shadow bias
	*/

	_rsmVolume->SetShadowBias (shadow.bias);

	/*
	 * Update statistics object
	*/

	auto rsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

	rsmStatisticsObject->rsmVolume = _rsmVolume;
}

std::vector<PipelineAttribute> RSMAccumulationRenderPass::GetCustomAttributes (const RenderLightObject* renderLightObject) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightPosition;
	PipelineAttribute lightDirection;
	PipelineAttribute lightColor;
	PipelineAttribute lightIntensity;

	lightPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	lightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	lightIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lightPosition.name = "lightPosition";
	lightDirection.name = "lightDirection";
	lightColor.name = "lightColor";
	lightIntensity.name = "lightIntensity";

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);
	lightDir = glm::normalize (lightDir);

	lightPosition.value = renderLightObject->GetTransform ()->GetPosition ();
	lightDirection.value = lightDir;
	lightColor.value = renderLightObject->GetLightColor ().ToVector3 ();
	lightIntensity.value.x = renderLightObject->GetLightIntensity ();

	attributes.push_back (lightPosition);
	attributes.push_back (lightDirection);
	attributes.push_back (lightColor);
	attributes.push_back (lightIntensity);

	return attributes;
}
