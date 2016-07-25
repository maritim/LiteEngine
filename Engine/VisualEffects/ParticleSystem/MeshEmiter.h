#ifndef MESHEMITER_H
#define MESHEMITER_H

#include "PrimitiveEmiter.h"

#include <vector>

#include "Particle.h"

#include "Mesh/Model.h"

class MeshSample
{
public:
	glm::vec3* a;
	glm::vec3* b;
	glm::vec3* c;
};

class MeshEmiter : public PrimitiveEmiter
{
protected:
	Model* _mesh;
	std::vector<MeshSample> _meshSamples;

public:
	MeshEmiter (Model* mesh);
	~MeshEmiter ();
protected:
	glm::vec3 GetParticlePosition ();
private:
	void ProcessObjectModel (Model* mesh, ObjectModel* objModel);
	void ProcessPolygonGroup (Model* mesh, PolygonGroup* polyGroup);
};

#endif