#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Systems/Components/Component.h"

#include <glm/vec2.hpp>

#include "Renderer/RenderLightObject.h"

#include "Utils/Color/Color.h"

class ENGINE_API LightComponent : public Component
{
	DECLARE_COMPONENT(LightComponent)

public:
	struct ShadowInformation
	{
		glm::ivec2 resolution;
		std::size_t cascadesCount;
		float bias;

		ShadowInformation ();
	};

protected:
	ATTRIBUTE(EditAnywhere, Meta=(Color))
	Color _color;
	ATTRIBUTE(EditAnywhere, Meta)
	float _intensity;
	ATTRIBUTE(EditAnywhere, Meta)
	bool _castShadows;
	ATTRIBUTE(EditAnywhere, Meta=(ShadowInformation))
	LightComponent::ShadowInformation _shadowInformation;

	RenderLightObject* _renderLightObject;

public:
	LightComponent ();
	~LightComponent ();

	void SetActive (bool isActive);

	Color GetColor () const;
	float GetIntensity () const;
	bool IsCastingShadows () const;
	LightComponent::ShadowInformation GetShadowInformation () const;

	virtual void SetColor (const Color& color);
	virtual void SetIntensity (float intensity);
	virtual void SetShadowCasting (bool castShadows);
	virtual void SetShadowInformation (const LightComponent::ShadowInformation& shadow);
};

#endif