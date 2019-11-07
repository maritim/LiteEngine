#ifndef RENDERSPOTLIGHTOBJECT_H
#define RENDERSPOTLIGHTOBJECT_H

#include "Renderer/RenderPointLightObject.h"

class RenderSpotLightObject : public RenderPointLightObject
{
protected:
	float _lightSpotCutoff;
	float _lightSpotOuterCutoff;

public:
	RenderSpotLightObject ();

	void SetLightSpotCutoff (float lightSpotCutoff);
	void SetLightSpotOuterCutoff (float lightSpotOuterCutoff);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif