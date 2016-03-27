#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "Core/Math/Vector3.h"
#include "Light.h"

class LightsManager : public Singleton<LightsManager>
{
	friend class Singleton<LightsManager>;

private:
	std::vector<Light*> _lights;
	Vector3 _ambientLight;
	
public:
	void AddLight (Light* light);
	void RemoveLight (Light* light);

	void SetAmbientColorLight (Vector3 ambientLight);
	Vector3 GetAmbientColorLight ();

	Light* GetLight (std::size_t index);
	std::size_t Size ();

	void Clear ();

private:
	LightsManager ();
	~LightsManager ();
	LightsManager (const LightsManager& other);
	LightsManager& operator= (const LightsManager& other);
};

#endif