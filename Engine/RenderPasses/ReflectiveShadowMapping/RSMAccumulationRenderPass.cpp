#include "RSMAccumulationRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Core/Console/Console.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

RSMAccumulationRenderPass::RSMAccumulationRenderPass () :
	_reflectiveShadowMapVolume (new RSMVolume ())
{

}

RSMAccumulationRenderPass::~RSMAccumulationRenderPass ()
{
	delete _reflectiveShadowMapVolume;
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
	_reflectiveShadowMapVolume->SetLightCamera (lightCamera);

	/*
	* Render geometry on shadow map
	*/

	ShadowMapGeometryPass (renderScene, lightCamera, settings, renderLightObject);

	/*
	* End shadow map drawing process
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ReflectiveShadowMapVolume", _reflectiveShadowMapVolume);
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
	_reflectiveShadowMapVolume->Clear ();
}

void RSMAccumulationRenderPass::StartShadowMapPass ()
{
	/*
	* Bind shadow map volume for writing
	*/

	_reflectiveShadowMapVolume->BindForWriting ();
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

	FrustumVolume* frustum = lightCamera->GetFrustumVolume ();

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

		if (renderObject->GetCollider () != nullptr) {
			GeometricPrimitive* primitive = renderObject->GetCollider ()->GetGeometricPrimitive ();
			if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
				continue;
			}
		}

		/*
		* Lock shader based on scene object layers
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Send custom attributes
		*/

		Pipeline::SendCustomAttributes ("", GetCustomAttributes (renderLightObject));

		/*
		* Render object on shadow map
		*/

		renderObject->Draw ();
	}
}

void RSMAccumulationRenderPass::EndShadowMapPass ()
{
	_reflectiveShadowMapVolume->EndDrawing ();
}

void RSMAccumulationRenderPass::InitRSMVolume (const RenderLightObject* renderLightObject)
{
	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (!_reflectiveShadowMapVolume->Init (shadow.resolution)) {
		Console::LogError (std::string () + "Reflective shadow map cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT);
	}
}

void RSMAccumulationRenderPass::UpdateRSMVolume (const RenderLightObject* renderLightObject)
{
	glm::ivec2 rsmSize = _reflectiveShadowMapVolume->GetSize ();

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (rsmSize != shadow.resolution) {

		/*
		 * Clear framebuffer
		*/

		_reflectiveShadowMapVolume->Clear ();

		/*
		 * Initialize framebuffer
		*/

		InitRSMVolume (renderLightObject);
	}

	_reflectiveShadowMapVolume->SetShadowBias (shadow.bias);

	StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
	RSMStatisticsObject* rsmStatisticsObject = nullptr;

	if (stat == nullptr) {
		stat = new RSMStatisticsObject ();
		StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
	}

	rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

	rsmStatisticsObject->rsmPosMapID = _reflectiveShadowMapVolume->GetShadowMapBuffer ()->GetColorBuffer (0);
	rsmStatisticsObject->rsmNormalMapID = _reflectiveShadowMapVolume->GetShadowMapBuffer ()->GetColorBuffer (1);
	rsmStatisticsObject->rsmFluxMapID = _reflectiveShadowMapVolume->GetShadowMapBuffer ()->GetColorBuffer (2);
	rsmStatisticsObject->rsmDepthMapID = _reflectiveShadowMapVolume->GetShadowMapBuffer ()->GetDepthBuffer ();
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
