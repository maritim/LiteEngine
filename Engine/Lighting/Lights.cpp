#include "Lights.h"

#include <vector>
#include <string>

Light::Light () :
	type (Type::POINT_LIGHT),
	position (),
	color (Vector3::One),
	specularColor (Vector3::One),
	constantAttenuation (1.0),
	linearAttenuation (0.1),
	quadraticAttenuation (0.01),
	spotCutoff (360.0),
	spotExponent (1.0),
	spotDirection (1.0, 1.0, 1.0)

{
	Lights::Instance ().AddLight (this);
}

Light::~Light ()
{
	Lights::Instance ().RemoveLight (this);
}

Lights::Lights () :
	_ambientLight (1.0, 1.0, 1.0)
{

}

Lights& Lights::Instance ()
{
	static Lights instance;

	return instance;
}

void Lights::AddLight (Light* light)
{
	_lights.push_back (light);
}

void Lights::RemoveLight (Light* light)
{
	for (std::size_t i=0;i<_lights.size();i++) {
		if (_lights [i] == light) {
			_lights [i] = _lights [_lights.size () - 1];
			_lights.pop_back ();

			break ;
		}
	}
}

void Lights::SetAmbientColorLight (Vector3 color)
{
	_ambientLight = color;
}

Vector3 Lights::GetAmbientColorLight ()
{
	return _ambientLight;
}

Light* Lights::GetLight (std::size_t index)
{
	if (index >= _lights.size()) {
		return NULL;
	}

	return _lights [index];
}

std::size_t Lights::Size ()
{
	return _lights.size ();
}

Lights::~Lights ()
{
	for (std::size_t i=0;i<_lights.size();i++) {
		delete _lights [i];
	}

	_lights.clear ();
	std::vector<Light*> ().swap (_lights);
}
