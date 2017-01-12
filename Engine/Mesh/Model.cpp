#include "Model.h"

#include <string>
#include <vector>
#include <cmath>

#include "Core/Math/glm/detail/func_geometric.hpp"

Model::Model() :
	_haveUV (false),
	_name (""),
	_mtllib (""),
	_vertices (),
	_objectModels (),
	_normals (),
	_texcoords (),
	_boundingBox (nullptr)
{
}

Model::Model(const Model& other) :
	_haveUV (other._haveUV),
	_name (other._name),
	_mtllib (other._mtllib),
	_vertices (),
	_objectModels (),
	_normals (),
	_texcoords (),
	_boundingBox (nullptr)
{
	for (std::size_t i=0;i<other._vertices.size();i++) {
		_vertices.push_back (new glm::vec3 (*other._vertices [i]));
	}

	for (std::size_t i=0;i<other._normals.size();i++) {
		_normals.push_back (new glm::vec3 (*other._normals [i]));
	}

	for (std::size_t i=0;i<other._texcoords.size();i++) {
		_texcoords.push_back (new glm::vec3 (*other._texcoords [i]));
	}

	for (std::size_t i=0;i<other._objectModels.size();i++) {
		_objectModels.push_back (new ObjectModel (*other._objectModels [i]));
	}
}

Model::~Model()
{
	for (std::size_t i=0;i<_vertices.size();i++) {
		delete _vertices[i];
	}
	_vertices.clear();
	_vertices.shrink_to_fit ();

	for (std::size_t i=0;i<_normals.size();i++) {
		delete _normals[i];
	}
	_normals.clear();
	_normals.shrink_to_fit ();

	for (std::size_t i=0;i<_texcoords.size();i++) {
		delete _texcoords[i];
	}
	_texcoords.clear();
	_texcoords.shrink_to_fit ();

	for (std::size_t i=0;i<_objectModels.size();i++) {
		delete _objectModels[i];
	}
	_objectModels.clear();
	_objectModels.shrink_to_fit ();

	if (_boundingBox != nullptr) {
		delete _boundingBox;
	}
}

void Model::AddVertex (glm::vec3* vertex) 
{
	_vertices.push_back (vertex);
}

void Model::AddNormal (glm::vec3* normal)
{
	_normals.push_back (normal);
}

void Model::AddTexcoord (glm::vec3* texcoord)
{
	_texcoords.push_back (texcoord);

	_haveUV = true;
}

void Model::AddObjectModel (ObjectModel* object)
{
	_objectModels.push_back (object);
}

void Model::SetName (std::string modelName)
{
	_name = modelName;
}

void Model::SetMaterialLibrary (std::string mtllibName)
{
	_mtllib = mtllibName;
}

void Model::SetVertex (glm::vec3* vertex, std::size_t position)
{
	delete _vertices [position];
	_vertices [position] = vertex;
}

bool Model::HaveUV (void) const {
	return _haveUV;
}

// Deprecated for the moment (26.12.2015)
void Model::Release()
{
	for (std::size_t i=0;i<_vertices.size();i++) {
		delete _vertices[i];
		delete _normals[i];
		delete _texcoords[i];
	}

	for (std::size_t i=0;i<_objectModels.size();i++)  {
		delete _objectModels[i];
	}
}

std::size_t Model::VertexCount (void) const 
{
	return _vertices.size ();
}

std::size_t Model::NormalsCount (void) const 
{
	return _normals.size ();
}

std::size_t Model::TexcoordsCount (void) const 
{
	return _texcoords.size ();
}

std::size_t Model::ObjectsCount (void) const 
{
	return _objectModels.size ();
}

glm::vec3* Model::GetVertex (std::size_t position) const 
{
	if (position >= _vertices.size()) {
		return nullptr;
	}

	return _vertices [position];
}

glm::vec3* Model::GetNormal (std::size_t position) const 
{
	if (position >= _normals.size()) {
		return NULL;
	}

	return _normals [position];
}

glm::vec3* Model::GetTexcoord (std::size_t position) const 
{
	if (position >= _texcoords.size()) {
		return NULL;
	}

	return _texcoords [position];
}

ObjectModel* Model::GetObject (std::size_t position) const 
{
	if (position >= _objectModels.size()) {
		return NULL;
	}

	return _objectModels [position];
}

ObjectModel* Model::GetObject (std::string objectName) const
{
	// TODO: Maybe optimize this.
	for (std::size_t i=0;i<_objectModels.size();i++) {
		if (_objectModels[i]->GetName () == objectName) {
			return _objectModels [i];
		}
	}

	return NULL;
}

std::string Model::GetName (void) const 
{
	return _name;
}

std::string Model::GetMaterialLibrary (void) const 
{
	return _mtllib;
}

BoundingBox* Model::GetBoundingBox ()
{
	if (_boundingBox == nullptr) {
		_boundingBox = CalculateBoundingBox ();
	}

	return _boundingBox;
}

void Model::ClearObjects (void) 
{
	for (std::size_t i=0;i<_objectModels.size();i++) {
		delete _objectModels [i];
	}

	_objectModels.clear ();
	_objectModels.shrink_to_fit ();
}

void Model::GenerateMissingNormals()
{
	for (std::size_t obji=0;obji<_objectModels.size ();obji++) {
		ObjectModel* objModel = _objectModels [obji];

		for (std::size_t i=0;i<objModel->GetPolygonCount ();i++) {
			PolygonGroup* polyGroup = objModel->GetPolygonGroup (i);

			for (std::size_t j=0;j<polyGroup->GetPolygonCount ();j++) {
				Polygon* poly = polyGroup->GetPolygon (j);

				if (poly->VertexCount () > 0 && !poly->HaveNormals ()) {
					glm::vec3* normal = CalculateNormal (poly);

					_normals.push_back (normal);

					for (std::size_t k=0;k<poly->VertexCount ();k++) {
						poly->AddNormal (_normals.size () - 1);
					}
				}
			}
		}
	}
}

void Model::GenerateSmoothNormals ()
{
	_smoothNormals.resize (_vertices.size (), glm::vec3 (0.0f));
	_smoothNormalsCount.resize (_vertices.size (), 0);

	GenerateMissingNormals ();

	for (std::size_t i=0;i<_objectModels.size ();i++) {
		ObjectModel* objModel = _objectModels [i];

		for (std::size_t j=0;j<objModel->GetPolygonCount ();j++) {
			PolygonGroup* polyGroup = objModel->GetPolygonGroup (j);

			for (std::size_t k=0;k<polyGroup->GetPolygonCount ();k++) {
				Polygon* poly = polyGroup->GetPolygon (k);

				for (std::size_t l=0;l<poly->VertexCount ();l++) {
					std::size_t vertex = poly->GetVertex (l);
					std::size_t normal = poly->GetNormal (l);

					_smoothNormals [vertex] += *(_normals [normal]);
					_smoothNormalsCount [vertex] ++;

					poly->SetNormal (vertex, l);
				}
			}
		}
	}

	for (std::size_t i=0;i<_smoothNormals.size ();i++) {
		_smoothNormals [i] *= (1.0f / _smoothNormalsCount [i]); 
		_smoothNormals [i] = glm::normalize (_smoothNormals [i]);
	}

	for (std::size_t i=0;i<_normals.size ();i++) {
		delete _normals [i];
	}
	_normals.clear ();
	_normals.shrink_to_fit ();

	for (std::size_t i=0;i<_smoothNormals.size ();i++) {
		_normals.push_back ( new glm::vec3(_smoothNormals [i]));
	}

	_smoothNormals.clear ();
	_smoothNormals.shrink_to_fit ();

	_smoothNormalsCount.clear ();
	_smoothNormalsCount.shrink_to_fit ();
}

// void Model::Repair () 
// {
// 	int *usedPoints = new int [VertexCount ()];
// 	int *usedTexcoords = new int [TexcoordsCount ()];
// 	int *usedNormals = new int [NormalsCount ()];

// 	for (std::size_t i=0;i<_vertices.size ();i++) {
// 		usedPoints [i] = false;
// 	}

// 	for (std::size_t i=0;i<_texcoords.size ();i++) {
// 		usedTexcoords [i] = false;
// 	}

// 	for (std::size_t i=0;i<_normals.size();i++) {
// 		usedNormals [i] = false;
// 	}

// 	for (std::size_t i=0;i<_faces.size();i++) {
// 		Polygon* face = _faces [i];
// 		for (std::size_t j=0;j<face->VertexCount ();j++) {
// 			usedPoints [face->GetVertex (j)] = true;
// 			if (_haveUV) {
// 				usedTexcoords [face->GetTexcoord (j)] = true;
// 			}
// 			usedNormals [face->GetNormal (j)] = true;
// 		}
// 	}

// 	std::size_t it = 0;
// 	for (std::size_t i=0;i<_vertices.size ();i++) {
// 		if (!usedPoints [i]) {
// 			delete _vertices [i];
// 		} else {
// 			_vertices [it++] = _vertices [i];
// 		}

// 		if (i == 0) {
// 			continue;
// 		}

// 		usedPoints [i] += usedPoints [i-1];
// 	}

// 	for (;it < _vertices.size ();) {
// 		_vertices.pop_back ();
// 	}

// 	it = 0;
// 	for (std::size_t i=0;i<_normals.size();i++) {
// 		if (!usedNormals [i]) {
// 			delete _normals [i];
// 		} else {
// 			_normals [it++] = _normals [i];
// 		}

// 		if (i == 0) {
// 			continue;
// 		}

// 		usedNormals [i] += usedNormals [i-1];
// 	}

// 	for (;it < _normals.size ();) {
// 		_normals.pop_back ();
// 	}

// 	if (_haveUV) {
// 		it = 0;

// 		for (std::size_t i=0;i<_texcoords.size();i++) {
// 			if (!usedTexcoords [i]) {
// 				delete _texcoords [i];
// 			} else {
// 				_texcoords [it++] = _texcoords [i];
// 			}

// 			if (i == 0) {
// 				continue;
// 			}

// 			usedTexcoords [i] += usedTexcoords [i-1];
// 		}

// 		for (;it <= _texcoords.size ();) {
// 			_texcoords.pop_back ();
// 		}
// 	}

// 	for (std::size_t i=0;i<_faces.size ();i++) {
// 		Polygon* face = _faces [i];
// 		for (std::size_t j=0;j<face->VertexCount ();j++) {
// 			face->SetVertex ( usedPoints [face->GetVertex (j)] - 1, j);
// 			face->SetNormal (usedNormals [face->GetNormal (j)] - 1, j);

// 			if (_haveUV) {
// 				face->SetTexcoord (usedTexcoords [face->GetTexcoord (j)] - 1, j);
// 			}
// 		}
// 	}

// 	delete[] usedPoints;
// 	delete[] usedTexcoords;
// 	delete[] usedNormals;
// }

// TODO: Reimplement this
glm::vec3* Model::CalculateNormal (Polygon* polygon)
{
/* 
	Set Vector U to (Triangle.p2 minus Triangle.p1)
	Set Vector V to (Triangle.p3 minus Triangle.p1)
 
	Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
	Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
	Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
 
	Returning Normal
*/ 

	glm::vec3 *first = _vertices[polygon->GetVertex(0)];
	glm::vec3 *second = _vertices[polygon->GetVertex(1)];
	glm::vec3 *third = _vertices[polygon->GetVertex(2)];

	glm::vec3 U = (*second) - (*first);
	glm::vec3 V = (*third) - (*first);

	glm::vec3 * normal = new glm::vec3();

	normal->x = (U.y * V.z) - (U.z * V.y);
	normal->y = (U.z * V.x) - (U.x * V.z);
	normal->z = (U.x * V.y) - (U.y * V.x);

	// Normalize the normal

	float val = sqrt (normal->x * normal->x  + normal->y * normal->y + normal->z * normal->z);

	normal->x /= val;
	normal->y /= val;
	normal->z /= val;

	return normal;
}

BoundingBox* Model::CalculateBoundingBox ()
{
	BoundingBox* boundingBox = new BoundingBox ();

	for (std::size_t obji=0;obji<_objectModels.size ();obji++) {
		ObjectModel* objModel = _objectModels [obji];

		for (std::size_t i=0;i<objModel->GetPolygonCount ();i++) {
			PolygonGroup* polyGroup = objModel->GetPolygonGroup (i);

			for (std::size_t j=0;j<polyGroup->GetPolygonCount ();j++) {
				Polygon* poly = polyGroup->GetPolygon (j);

				for (std::size_t k=0;k<poly->VertexCount ();k++) {
					glm::vec3* vertex = _vertices [poly->GetVertex (k)];

					boundingBox->xmin = std::min (boundingBox->xmin, vertex->x);
					boundingBox->xmax = std::max (boundingBox->xmax, vertex->x);

					boundingBox->ymin = std::min (boundingBox->ymin, vertex->y);
					boundingBox->ymax = std::max (boundingBox->ymax, vertex->y);

					boundingBox->zmin = std::min (boundingBox->zmin, vertex->z);
					boundingBox->zmax = std::max (boundingBox->zmax, vertex->z);
				}
			}
		}
	}

	return boundingBox;	
}
