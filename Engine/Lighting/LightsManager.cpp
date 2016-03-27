#include "LightsManager.h"

#include <vector>
#include <string>

LightsManager::LightsManager () :
	_ambientLight (Vector3::One)
{

}

LightsManager::~LightsManager ()
{
	for (std::size_t i=0;i<_lights.size();i++) {
		delete _lights [i];
	}

	_lights.clear ();
	std::vector<Light*> ().swap (_lights);
}

void LightsManager::AddLight (Light* light)
{
	_lights.push_back (light);
}

void LightsManager::RemoveLight (Light* light)
{
	for (std::size_t i=0;i<_lights.size();i++) {
		if (_lights [i] == light) {
			_lights [i] = _lights [_lights.size () - 1];
			_lights.pop_back ();

			break ;
		}
	}
}

void LightsManager::SetAmbientColorLight (Vector3 color)
{
	_ambientLight = color;
}

Vector3 LightsManager::GetAmbientColorLight ()
{
	return _ambientLight;
}

Light* LightsManager::GetLight (std::size_t index)
{
	if (index >= _lights.size()) {
		return NULL;
	}

	return _lights [index];
}

std::size_t LightsManager::Size ()
{
	return _lights.size ();
}
