#ifndef FONTQUAD_H
#define FONTQUAD_H

#include "Core/Interfaces/Object.h"

#include "Core/Resources/Resource.h"
#include "Mesh/Model.h"

class FontQuad
{
private:
	Resource<Model> _quad;

public:
	FontQuad ();
	~FontQuad ();

	void SetPosition (Vector3 a, Vector3 b, Vector3 c, Vector3 d);
	void SetTexcoord (Vector3 a, Vector3 b, Vector3 c, Vector3 d);
};

#endif