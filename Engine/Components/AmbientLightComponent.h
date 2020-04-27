#ifndef AMBIENTLIGHTCOMPONENT_H
#define AMBIENTLIGHTCOMPONENT_H

#include "Systems/Components/Component.h"

#include "Renderer/RenderAmbientLightObject.h"

class ENGINE_API AmbientLightComponent : public Component
{
	DECLARE_COMPONENT(AmbientLightComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta=(Color))
	Color _color;
	ATTRIBUTE(EditAnywhere, Meta)
	float _intensity;

	RenderAmbientLightObject* _renderLightObject;

public:
	AmbientLightComponent ();
	~AmbientLightComponent ();

	void Awake ();

	void SetActive (bool isActive);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	Color GetColor () const;
	float GetIntensity () const;

	void SetColor (const Color& color);
	void SetIntensity (float intensity);
};

#endif