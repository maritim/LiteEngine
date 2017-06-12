#include "DeferredLightRenderPass.h"

#include "Lighting/LightsManager.h"

#include "GBuffer.h"

#include "Wrappers/OpenGL/GL.h"

DeferredLightRenderPass::~DeferredLightRenderPass ()
{

}

void DeferredLightRenderPass::Init ()
{

}

RenderVolumeCollection* DeferredLightRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Draw volumetric lights
	*/

	LightPass (scene, camera, rvc);

	return rvc;
}

void DeferredLightRenderPass::LightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	((GBuffer*)rvc->GetRenderVolume("GBuffer"))->BindForLightPass ();

	DirectionalLightPass (scene, camera, rvc);
	PointLightPass (scene, camera, rvc);
}

void DeferredLightRenderPass::DirectionalLightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	GL::Disable (GL_DEPTH_TEST);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	for (std::size_t i = 0; i<LightsManager::Instance ()->GetDirectionalLightsCount (); i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetDirectionalLight (i);

		if (!volumetricLight->IsActive ()) {
			continue;
		}

		volumetricLight->GetLightRenderer ()->Draw (scene, camera, rvc);
	}

	GL::Disable (GL_BLEND);
	GL::Enable (GL_DEPTH_TEST);
}

void DeferredLightRenderPass::PointLightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	for (std::size_t i = 0; i<LightsManager::Instance ()->GetPointLightsCount (); i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetPointLight (i);

		if (!volumetricLight->IsActive ()) {
			continue;
		}

		PointLightStencilPass (volumetricLight, rvc);
		PointLightDrawPass (volumetricLight, rvc);
	}
}

void DeferredLightRenderPass::PointLightStencilPass (VolumetricLight* volumetricLight, RenderVolumeCollection* rvc)
{
	((GBuffer*)rvc->GetRenderVolume ("GBuffer"))->BindForStencilPass ();

	/*
	* Don't need to write the light on depth buffer.
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	* Stencil pass technique. For more details please look at the link bellow.
	* http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html
	*
	* It uses stencil test to identify the fragments that are in light.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 0, 0);

	GL::StencilOpSeparate (GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	GL::StencilOpSeparate (GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	/*
	* Both faces need to be drawn.
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	* Volumetric light draw.
	*/

	volumetricLight->GetLightRenderer ()->Draw (nullptr, nullptr);

	/*
	* Reset the settings.
	*/

	GL::DepthMask (GL_TRUE);

	GL::Enable (GL_CULL_FACE);
	GL::Disable (GL_STENCIL_TEST);
}

void DeferredLightRenderPass::PointLightDrawPass (VolumetricLight* volumetricLight, RenderVolumeCollection* rvc)
{
	((GBuffer*)rvc->GetRenderVolume ("GBuffer"))->BindForLightPass ();

	/*
	* Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	* Pass the fragment only if it is like that
	*
	* 				(buffer frag) > (value '1')
	*
	* Don't override the sky, which have value '0'.
	* Override only what fragments that are in light.
	*
	* After drawing, Stencil Buffer is refilled with 1's.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_LESS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	* Blend between point lights with same weight.
	*/

	GL::Enable (GL_BLEND);
	GL::BlendEquation (GL_FUNC_ADD);
	GL::BlendFunc (GL_ONE, GL_ONE);

	/*
	* Process fragments single time. Front Face randomly chosen.
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_FRONT);

	/*
	* Draw the volumetric light.
	*/

	volumetricLight->GetLightRenderer ()->Draw (nullptr, nullptr);

	/*
	* Reset the settings.
	*/

	GL::CullFace (GL_BACK);

	GL::Disable (GL_STENCIL_TEST);
	GL::Disable (GL_BLEND);

	GL::DepthMask (GL_TRUE);
	GL::Enable (GL_DEPTH_TEST);
}

