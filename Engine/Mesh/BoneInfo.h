#ifndef BONEINFO_H
#define BONEINFO_H

#include "Core/Interfaces/Object.h"

#include <glm/glm.hpp>
#include <string>

class BoneInfo : public Object
{
protected:
	std::string _name;
	std::size_t _id;
	glm::mat4 _transform;

public:
	BoneInfo ();

	void SetName (const std::string& name);
	std::string GetName () const;

	void SetID (std::size_t id);
	std::size_t GetID () const;

	void SetTransformMatrix (const glm::mat4& transformMatrix);
	glm::mat4 GetTransformMatrix () const;
};

#endif