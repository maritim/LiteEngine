#include "LightsManager.h"

#include <algorithm>

LightsManager::LightsManager () :
	_ambientLight (Color::Black)
{

}

LightsManager::~LightsManager ()
{

}

void LightsManager::SetAmbientLightColor (Color color)
{
	_ambientLight = color;
}

Color LightsManager::GetAmbientLightColor () const
{
	return _ambientLight;
}

void LightsManager::AddDirectionalLight (DirectionalLight* light)
{
	_directionalLights.push_back (light);
}

void LightsManager::AddPointLight (PointLight* light)
{
	_pointLights.push_back (light);
}

void LightsManager::AddSpotLight (SpotLight* light)
{
	_spotLights.push_back (light);
}

void LightsManager::RemoveDirectionalLight (DirectionalLight* light)
{
	auto it = std::find (_directionalLights.begin (), _directionalLights.end (), light);

	if (it == _directionalLights.end ()) {
		return ;
	}

	_directionalLights.erase (it);
}

void LightsManager::RemovePointLight (PointLight* light)
{
	auto it = std::find (_pointLights.begin (), _pointLights.end (), light);

	if (it == _pointLights.end ()) {
		return ;
	}

	_pointLights.erase (it);
}

void LightsManager::RemoveSpotLight (SpotLight* light)
{
	auto it = std::find (_spotLights.begin (), _spotLights.end (), light);

	if (it == _spotLights.end ()) {
		return ;
	}

	_spotLights.erase (it);
}

std::size_t LightsManager::GetDirectionalLightsCount () const
{
	return _directionalLights.size ();
}

std::size_t LightsManager::GetPointLightsCount () const
{
	return _pointLights.size ();
}

std::size_t LightsManager::GetSpotLightsCount () const
{
	return _spotLights.size ();
}
