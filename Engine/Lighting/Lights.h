#ifndef LIGHTS_H
#define LIGHTS_H

#include <vector>

#include "Core/Math/Vector3.h"

class Light
{
public:
	enum Type { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

public:
	Type type;

	Vector3 position;
	Vector3 color;
	Vector3 specularColor;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	Vector3 spotDirection;

public:
	Light ();
	~Light ();
};

class Lights
{
private:
	std::vector<Light*> _lights;
	Vector3 _ambientLight;
public:
	static Lights& Instance ();

	void AddLight (Light* light);
	void RemoveLight (Light* light);

	void SetAmbientColorLight (Vector3 ambientLight);
	Vector3 GetAmbientColorLight ();

	Light* GetLight (std::size_t index);
	std::size_t Size ();

	void Clear ();

	~Lights ();

private:
	Lights ();
};

#endif