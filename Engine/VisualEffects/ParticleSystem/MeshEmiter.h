#ifndef MESHEMITER_H
#define MESHEMITER_H

#include "PrimitiveEmiter.h"

#include <vector>

#include "Particle.h"

#include "Mesh/Model.h"

#include "Core/Math/Vector3.h"

class MeshSample
{
public:
	Vector3* a;
	Vector3* b;
	Vector3* c;
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
	virtual Vector3 GetParticlePosition ();
private:
	void ProcessObjectModel (Model* mesh, ObjectModel* objModel);
	void ProcessPolygonGroup (Model* mesh, PolygonGroup* polyGroup);
};

#endif