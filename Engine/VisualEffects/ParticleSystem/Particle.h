#ifndef PARTICLE_H
#define PARTICLE_H

#include "SceneGraph/SceneObject.h"

#include <glm/vec3.hpp>

#include "Core/Resources/Resource.h"
#include "Mesh/Model.h"
#include "SceneGraph/Transform.h"

#include "Utils/Curves/AnimationCurve.h"

class Particle : public SceneObject
{
protected:
	// In milliseconds
	unsigned int _lifetime;
	// Movement direction
	glm::vec3* _direction;
	// Movement speed - units per second
	float _speed;

	AnimationCurve* _scaleCurve;
	AnimationCurve* _tweenCurve;

	bool _alive;

	glm::vec3 _initialPosition;
	glm::vec3 _finalDestination;
	glm::vec3 _initialScale;
	unsigned int _timeAlive;

	Resource<Model> _mesh;

public:
	Particle ();
	Particle (const Particle& other);
	virtual ~Particle ();

	void SetLifetime (unsigned int lifetime);
	void SetScaleCurve (AnimationCurve* scaleCurve);
	void SetTweenCurve (AnimationCurve* tweenCurve);
	void SetSpeed (float speed);
	void SetMoveDirection (glm::vec3* direction);

	void Init ();

	void SetMesh (const Resource<Model>& mesh);
	Resource<Model> GetMesh () const;

	bool IsAlive () const;

	void Update ();

	virtual Particle* Clone ();
protected:
	void UpdatePosition ();
	void UpdateScale ();
	void UpdateRotation ();
};

#endif