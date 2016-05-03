#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "Core/Math/Vector3.h"

#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class LightsManager : public Singleton<LightsManager>
{
	friend class Singleton<LightsManager>;

private:
	std::vector<DirectionalLight*> _directionalLights;
	std::vector<PointLight*> _pointLights;
	std::vector<SpotLight*> _spotLights;

	Vector3 _ambientLight;
	
public:
	void SetAmbientColorLight (Vector3 ambientLight);
	Vector3 GetAmbientColorLight ();

	void AddDirectionalLight (DirectionalLight* light);
	void AddPointLight (PointLight* light);
	void AddSpotLight (SpotLight* light);

	DirectionalLight* GetDirectionalLight (std::size_t index);
	PointLight* GetPointLight (std::size_t index);
	SpotLight* GetSpotLight (std::size_t index);

	void RemoveDirectionalLight (DirectionalLight* light);
	void RemovePointLight (PointLight* light);
	void RemoveSpotLight (SpotLight* light);

	std::size_t GetDirectionalLightsCount ();
	std::size_t GetPointLightsCount ();
	std::size_t GetSpotLightsCount ();

private:
	LightsManager ();
	~LightsManager ();
	LightsManager (const LightsManager& other);
	LightsManager& operator= (const LightsManager& other);
};

#endif