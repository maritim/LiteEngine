#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Utils/Color/Color.h"

class LightsManager : public Singleton<LightsManager>
{
	friend class Singleton<LightsManager>;

private:
	std::vector<DirectionalLight*> _directionalLights;
	std::vector<PointLight*> _pointLights;
	std::vector<SpotLight*> _spotLights;

	Color _ambientLight;
	
public:
	void SetAmbientColorLight (Color ambientLight);
	Color GetAmbientColorLight () const;

	void AddDirectionalLight (DirectionalLight* light);
	void AddPointLight (PointLight* light);
	void AddSpotLight (SpotLight* light);

	void RemoveDirectionalLight (DirectionalLight* light);
	void RemovePointLight (PointLight* light);
	void RemoveSpotLight (SpotLight* light);

	std::size_t GetDirectionalLightsCount () const;
	std::size_t GetPointLightsCount () const;
	std::size_t GetSpotLightsCount () const;

	MULTIPLE_CONTAINER_TEMPLATE

private:
	LightsManager ();
	~LightsManager ();
	LightsManager (const LightsManager& other);
	LightsManager& operator= (const LightsManager& other);
};

MULTIPLE_CONTAINER_SPECIALIZATION (DirectionalLight*, LightsManager, _directionalLights);
MULTIPLE_CONTAINER_SPECIALIZATION (PointLight*, LightsManager, _pointLights);
MULTIPLE_CONTAINER_SPECIALIZATION (SpotLight*, LightsManager, _spotLights);

#endif