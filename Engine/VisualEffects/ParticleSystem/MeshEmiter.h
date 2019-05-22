#ifndef MESHEMITER_H
#define MESHEMITER_H

#include "PrimitiveEmiter.h"

#include <vector>

#include "Particle.h"

#include "Core/Resources/Resource.h"
#include "Mesh/Model.h"

class MeshSample
{
public:
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
};

class MeshEmiter : public PrimitiveEmiter
{
protected:
	Resource<Model> _mesh;
	std::vector<MeshSample> _meshSamples;

public:
	MeshEmiter (const Resource<Model>& mesh);
	~MeshEmiter ();
protected:
	glm::vec3 GetParticlePosition ();
private:
	void ProcessObjectModel (const Resource<Model>& mesh, ObjectModel* objModel);
	void ProcessPolygonGroup (const Resource<Model>& mesh, PolygonGroup* polyGroup);
};

#endif