/*************************************************************************** 
  Model Object
 ***************************************************************************/
 
#ifndef MODEL_H
#define MODEL_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <vector>

#include "Core/Math/glm/vec3.hpp"
#include "Core/Iteration/MultipleContainer.h"

#include "BoundingBox.h"

#include "Polygon.h"
#include "ObjectModel.h"

class Model : public Object
{
protected:
	bool _haveUV;
	std::string _name;
	std::string _mtllib;

	// Make this more flexible
	std::vector<glm::vec3> _vertices;
	std::vector<ObjectModel*> _objectModels;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texcoords;

	// Used for normal smoothing
	std::vector<glm::vec3> _smoothNormals;
	std::vector<int> _smoothNormalsCount;

	BoundingBox* _boundingBox;

public: 
	Model();			
	Model(const Model& other);
	void Release ();
	bool HaveUV () const;

	void AddNormal (const glm::vec3& normal);
	void AddVertex (const glm::vec3& vertex);
	void AddTexcoord (const glm::vec2& texcoord);
	void AddObjectModel (ObjectModel* object);

	void SetName (const std::string& modelName);
	void SetMaterialLibrary (const std::string& mtllibName);

	std::size_t VertexCount () const;
	std::size_t NormalsCount () const;
	std::size_t TexcoordsCount () const;
	std::size_t ObjectsCount () const;

	std::string GetName () const;
	std::string GetMaterialLibrary () const;

	glm::vec3 GetVertex (std::size_t position) const;
	glm::vec3 GetNormal (std::size_t position) const;
	glm::vec2 GetTexcoord (std::size_t position) const;
	ObjectModel* GetObject (std::string objectName) const;

	BoundingBox* GetBoundingBox ();

	void SetVertex (const glm::vec3& vertex, std::size_t position);
	void ClearObjects ();

	void GenerateMissingNormals ();
	void GenerateSmoothNormals ();
	void Repair ();

	MULTIPLE_CONTAINER_TEMPLATE

	~Model ();

protected:
	glm::vec3 CalculateNormal(Polygon* poly);

	BoundingBox* CalculateBoundingBox ();
};

MULTIPLE_CONTAINER_SPECIALIZATION (ObjectModel*, Model, _objectModels);
MULTIPLE_CONTAINER_SPECIALIZATION (glm::vec3, Model, _vertices);

#endif