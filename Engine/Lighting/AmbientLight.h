#ifndef AMBIENT_LIGHT
#define AMBIENT_LIGHT

#include "Light.h"

#include "Renderer/RenderAmbientLightObject.h"

class AmbientLight : public Light
{
protected:
	RenderAmbientLightObject* _renderLightObject;

public:
	AmbientLight ();
	~AmbientLight ();

	void SetActive (bool isActive);

	void SetColor (const Color& color);
	void SetIntensity (float intensity);

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif