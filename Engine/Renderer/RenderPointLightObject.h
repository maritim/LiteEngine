#ifndef RENDERPOINTLIGHTOBJECT_H
#define RENDERPOINTLIGHTOBJECT_H

#include "Renderer/RenderLightObject.h"

#include <glm/vec3.hpp>

class RenderPointLightObject : public RenderLightObject
{
protected:
	glm::vec3 _attenuation;

public:
	RenderPointLightObject ();

	void SetLightAttenuation (const glm::vec3& attenuation);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif