#include "AmbientLight.h"

#include "LightsManager.h"

AmbientLight::AmbientLight ()
{
	_color = Color::Black;
}

void AmbientLight::Update ()
{
	
}

void AmbientLight::OnAttachedToScene ()
{
	/*
	 * TODO: Solve this
	*/

	if (_isActive == false) {
		return;
	}

	LightsManager::Instance ()->SetAmbientLightColor (_color);
}
