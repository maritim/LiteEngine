#ifndef LIGHT_H
#define LIGHT_H

#include "SceneGraph/SceneObject.h"

#include "Utils/Color/Color.h"
#include "Core/Math/Vector3.h"

/*
 * Type: DIRECTIONAL / POINT / SPOT
 *
 * Color: light color
 *
 * Specular Color: may differ to color
 *
 * Attenuation: Distance factor
 *		x: Constant Attenunation
 *		y: Linear Attenuation
 *		z: Quadratic Attenuation
 *
 * Spot Direction: Direction for Lightning. Only used available for Spot Lights
*/

class Light : public SceneObject
{
public:
	enum Type { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

protected:
	Type _type;

	Color _color;
	Color _specularColor;
	float _constantAttenuation, _linearAttenuation, _quadraticAttenuation;
	float _spotCutoff, _spotExponent;
	Vector3 _spotDirection;

public:
	Light ();
	~Light ();

	Type GetType () const;
	Color GetColor () const;
	Color GetSpecularColor () const;
	Vector3 GetSpotDirection () const;
	float GetConstantAttenuation () const;
	float GetLinearAttenuation () const;
	float GetQuadraticAttenuation () const;
	float GetSpotCutoff () const;
	float GetSpotExponent () const;

	void SetType (Type type);
	void SetColor (const Color& color);
	void SetSpecularColor (const Color& specularColor);
	void SetConstantAttenuation (float constantAttenuation);
	void SetLinearAttenuation (float linearAttenuation);
	void SetQuadraticAttenuation (float quadraticAttenuation);
	void SetSpotDirection (const Vector3& spotDirection);
	void SetSpotCutoff (float spotCutoff);
	void SetSpotExponent (float spotExponent);

	void Update ();

	virtual void OnAttachedToScene ();
	virtual void OnDetachedFromScene ();
};

#endif