#include "LightsManager.h"

#include <vector>
#include <string>
#include <algorithm>

LightsManager::LightsManager () :
	_ambientLight (Vector3::One)
{

}

LightsManager::~LightsManager ()
{

}

void LightsManager::SetAmbientColorLight (Vector3 color)
{
	_ambientLight = color;
}

Vector3 LightsManager::GetAmbientColorLight ()
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

DirectionalLight* LightsManager::GetDirectionalLight (std::size_t index)
{
	if (index >= _directionalLights.size()) {
		return nullptr;
	}

	return _directionalLights [index];
}

PointLight* LightsManager::GetPointLight (std::size_t index)
{
	if (index >= _pointLights.size ()) {
		return nullptr;
	}

	return _pointLights [index];
}

SpotLight* LightsManager::GetSpotLight (std::size_t index)
{
	if (index >= _spotLights.size ()) {
		return nullptr;
	}

	return _spotLights [index];
}

std::size_t LightsManager::GetDirectionalLightsCount ()
{
	return _directionalLights.size ();
}

std::size_t LightsManager::GetPointLightsCount ()
{
	return _pointLights.size ();
}

std::size_t LightsManager::GetSpotLightsCount ()
{
	return _spotLights.size ();
}
