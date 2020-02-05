#include "RenderSkyboxObject.h"

#include "Renderer/Pipeline.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

RenderSkyboxObject::RenderSkyboxObject () :
    _shaderView (nullptr),
    _cubemapView (nullptr),
    _tintColor (Color::White),
    _brightness (0.0),
    _angularVelocity (0.0)
{
	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/Skybox/skyboxVertex.glsl",
		"Assets/Shaders/Skybox/skyboxFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

RenderSkyboxObject::~RenderSkyboxObject ()
{

}

void RenderSkyboxObject::Draw ()
{
	Pipeline::LockShader (_shaderView);

	Pipeline::SetObjectTransform (Transform::Default ());

	Pipeline::SendCustomAttributes (_shaderView, GetCustomAttributes ());

	_modelView->DrawGeometry ();

	Pipeline::UnlockShader ();
}

void RenderSkyboxObject::SetCubeMap (const Resource<TextureView>& cubemapView)
{
    _cubemapView = cubemapView;
}

void RenderSkyboxObject::SetTintColor (Color tintColor)
{
    _tintColor = tintColor;
}

void RenderSkyboxObject::SetBrightness (float brightness)
{
    _brightness = brightness;
}

void RenderSkyboxObject::SetAngularVelocity (float velocity)
{
    _angularVelocity = velocity;
}

std::vector<PipelineAttribute> RenderSkyboxObject::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute uniformCubeMap, uniformTintColor, uniformBrightness;

	uniformCubeMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_CUBE;
	uniformTintColor.type = PipelineAttribute::AttrType::ATTR_3F;
	uniformBrightness.type = PipelineAttribute::AttrType::ATTR_1F;

	uniformCubeMap.name = "cubeMap";
	uniformTintColor.name = "tintColor";
	uniformBrightness.name = "brightness";

	uniformCubeMap.value.x = _cubemapView->GetGPUIndex ();
	uniformTintColor.value = _tintColor.ToVector3 ();
	uniformBrightness.value.x = _brightness;

	attributes.push_back (uniformCubeMap);
	attributes.push_back (uniformTintColor);
	attributes.push_back (uniformBrightness);

	return attributes;
}
