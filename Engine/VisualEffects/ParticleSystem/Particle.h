#ifndef PARTICLE_H
#define PARTICLE_H

#include "SceneGraph/SceneObject.h"

#include <glm/vec3.hpp>

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Mesh/Model.h"
#include "SceneGraph/Transform.h"

#include "Utils/Curves/AnimationCurve.h"

#include "Renderer/BufferAttribute.h"
#include "Renderer/PipelineAttribute.h"

class Particle : public SceneObject
{
protected:
	// In milliseconds
	unsigned int _lifetime;
	// Movement direction
	glm::vec3 _direction;
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
	void SetMoveDirection (const glm::vec3& direction);

	void Init ();

	void SetMesh (const Resource<Model>& mesh);
	Resource<Model> GetMesh () const;

	virtual std::vector<BufferAttribute> GetBufferAttributes () const = 0;
	virtual unsigned char* GetBuffer () = 0;
	virtual std::size_t GetSize () const = 0;
	virtual std::string GetShaderName () const = 0;
	virtual std::vector<PipelineAttribute> GetAttributes () const = 0;

	bool IsAlive () const;

	void Update ();

	virtual Particle* Clone () = 0;
protected:
	void UpdatePosition ();
	void UpdateScale ();
	void UpdateRotation ();
};

#endif