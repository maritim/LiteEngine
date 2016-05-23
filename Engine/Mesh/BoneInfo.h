#ifndef BONEINFO_H
#define BONEINFO_H

#include "Core/Interfaces/Object.h"

#include <string>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class BoneInfo : public Object
{
protected:
	std::string _name;
	std::size_t _id;
	aiMatrix4x4 _transform;

public:
	BoneInfo ();

	void SetName (const std::string& name);
	std::string GetName () const;

	void SetID (std::size_t id);
	std::size_t GetID () const;

	void SetTransformMatrix (aiMatrix4x4 transformMatrix);
	aiMatrix4x4 GetTransformMatrix () const;
};

#endif