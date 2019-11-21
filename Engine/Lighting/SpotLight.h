#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "PointLight.h"

class ENGINE_API SpotLight : public PointLight
{
protected:
	float _spotCutoff;
	float _spotOuterCutoff;

public:
	SpotLight ();

	float GetSpotCutoff () const;
	float GetSpotOuterCutoff () const;

	void SetSpotCutoff (float spotCutoff);
	void SetSpotOuterCutoff (float spotOuterCutoff);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
protected:
	void UpdateTransform ();
};

#endif