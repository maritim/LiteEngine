#include "LightShadowMapRenderer.h"

LightShadowMapRenderer::LightShadowMapRenderer (Light* light) :
	LightRenderer (light),
	_volume (nullptr)
{

}

LightShadowMapRenderer::~LightShadowMapRenderer ()
{
	delete _volume;
}

void LightShadowMapRenderer::Draw (Scene* scene, Camera* camera)
{
	/*
	 * Shadow Pass
	*/

	ShadowMapPass (scene, camera);

	/*
	 * Object Rendering Pass
	*/

	RenderingPass (scene, camera);
}

/*
 * Need to be specialized for every light type
*/

void LightShadowMapRenderer::ShadowMapPass (Scene* scene, Camera* camera)
{
	/*
	 * Prepare for drawing
	*/

	_volume->BindForShadowMapCatch ();

	/*
	 * Draw shadow map
	*/

	ShadowMapRender (scene, camera);

	/*
	 * End drawing
	*/

	_volume->EndDrawing ();	
}

void LightShadowMapRenderer::RenderingPass (Scene* scene, Camera* camera)
{
	LightRenderer::Draw (scene, camera);
}

void LightShadowMapRenderer::ShadowMapRender (Scene* scene, Camera* camera)
{

}

