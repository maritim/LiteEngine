/*************************************************************************** 
  Model Object
 ***************************************************************************/
 
#ifndef MODEL_H
#define MODEL_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <vector>

#include "Core/Math/Vector3.h"

#include "Polygon.h"
#include "ObjectModel.h"

class Model : public Object
{
protected:
	bool _haveUV;
	std::string _name;
	std::string _mtllib;

	// Make this more flexible
	std::vector<Vector3*> _vertices;
	std::vector<ObjectModel*> _objectModels;
	std::vector<Vector3*> _normals;
	std::vector<Vector3*> _texcoords;

	// Use for normal smoothing
	std::vector<Vector3> _smoothNormals;
	std::vector<int> _smoothNormalsCount;

public: 
	Model();			
	Model(const Model& other);
	void Release();					
	bool HaveUV () const;

	void AddNormal (Vector3* normal);
	void AddVertex (Vector3* vertex);
	void AddTexcoord (Vector3* texcoord);
	void AddObjectModel (ObjectModel* object);

	void SetName (std::string modelName);
	void SetMaterialLibrary (std::string mtllibName);

	std::size_t VertexCount () const;
	std::size_t NormalsCount () const;
	std::size_t TexcoordsCount () const;
	std::size_t ObjectsCount () const;

	std::string GetName () const;
	std::string GetMaterialLibrary () const;

	Vector3* GetVertex (std::size_t position) const;
	Vector3* GetNormal (std::size_t position) const;
	Vector3* GetTexcoord (std::size_t position) const;
	ObjectModel* GetObject (std::size_t position) const;
	ObjectModel* GetObject (std::string objectName) const;

	void SetVertex (Vector3* vertex, std::size_t position);
	void ClearObjects ();

	void GenerateMissingNormals ();
	void GenerateSmoothNormals ();
	void Repair ();

	~Model();

protected:
	Vector3* CalculateNormal(Polygon* poly);
};
 
#endif