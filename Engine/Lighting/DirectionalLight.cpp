#include "DirectionalLight.h"

#include "LightsManager.h"

#include "Utils/Primitives/Primitive.h"

#include "Lighting/DirectionalLightRenderer.h"
#include "Shadows/DirectionalLightShadowMapRenderer.h"
#include "Illumination/VoxelConeTrace/DirectionalLightVoxelConeTraceRenderer.h"
#include "Illumination/ReflectiveShadowMap/DirectionalLightReflectiveShadowMapRenderer.h"

DirectionalLight::DirectionalLight ()
{
	SetVolume (Primitive::Instance ()->Create (Primitive::Type::QUAD));
}

void DirectionalLight::Update ()
{
	
}

void DirectionalLight::SetShadowCasting (bool casting)
{
	ChangeShadowCasting (casting);
}

void DirectionalLight::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddDirectionalLight (this);
}

void DirectionalLight::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemoveDirectionalLight (this);
}

void DirectionalLight::ChangeShadowCasting (bool casting)
{
	if (_castShadows == casting) {
		return;
	}

	_castShadows = casting;

	delete _lightRenderer;

	if (_castShadows == true) {
		_lightRenderer = new DirectionalLightShadowMapRenderer (this);
	} else {
		_lightRenderer = new DirectionalLightRenderer (this);
	}

	SetVolume (Primitive::Instance ()->Create (Primitive::Type::QUAD));
}

void DirectionalLight::SetRenderMode (LightRenderMode lightMode)
{
	if (lightMode == LightRenderMode::VOXEL_CONE_TRACE) {
		delete _lightRenderer;

		_lightRenderer = new DirectionalLightVoxelConeTraceRenderer (this);
	}

	if (lightMode == LightRenderMode::REFLECTIVE_SHADOW_MAP) {
		delete _lightRenderer;

		_lightRenderer = new DirectionalLightReflectiveShadowMapRenderer (this);
	}

	SetVolume (Primitive::Instance ()->Create (Primitive::Type::QUAD));
}