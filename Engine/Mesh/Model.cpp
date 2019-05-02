#include "Model.h"

#include <glm/geometric.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "Utils/Extensions/VectorExtend.h"

#include "Core/Console/Console.h"

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
	_vertices (other._vertices),
	_objectModels (),
	_normals (other._normals),
	_texcoords (other._texcoords),
	_boundingBox (nullptr)
{
	for (std::size_t i=0;i<other._objectModels.size();i++) {
		_objectModels.push_back (new ObjectModel (*other._objectModels [i]));
	}
}

Model::~Model()
{
	for (std::size_t i=0;i<_objectModels.size();i++) {
		delete _objectModels[i];
	}

	if (_boundingBox != nullptr) {
		delete _boundingBox;
	}
}

void Model::AddVertex (const glm::vec3& vertex)
{
	_vertices.push_back (vertex);
}

void Model::AddNormal (const glm::vec3& normal)
{
	_normals.push_back (normal);
}

void Model::AddTexcoord (const glm::vec2& texcoord)
{
	_texcoords.push_back (texcoord);

	_haveUV = true;
}

void Model::AddObjectModel (ObjectModel* object)
{
	_objectModels.push_back (object);
}

void Model::SetName (const std::string& modelName)
{
	_name = modelName;
}

void Model::SetMaterialLibrary (const std::string& mtllibName)
{
	_mtllib = mtllibName;
}

void Model::SetVertex (const glm::vec3& vertex, std::size_t position)
{
	_vertices [position] = vertex;
}

bool Model::HaveUV (void) const {
	return _haveUV;
}

// Deprecated for the moment (26.12.2015)
void Model::Release()
{
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

glm::vec3 Model::GetVertex (std::size_t position) const 
{
	if (position >= _vertices.size()) {
		Console::LogWarning ("Vertex index exceed vertices count. \
			You are searching for " + std::to_string (position) + " and the size is " +
			std::to_string (_vertices.size ()));

		return glm::vec3 ();
	}

	return _vertices [position];
}

glm::vec3 Model::GetNormal (std::size_t position) const 
{
	if (position >= _normals.size()) {
		Console::LogWarning ("Normal index exceed normals count. \
			You are searching for " + std::to_string (position) + " and the size is " +
			std::to_string (_normals.size ()));

		return glm::vec3 ();
	}

	return _normals [position];
}

glm::vec2 Model::GetTexcoord (std::size_t position) const 
{
	if (position >= _texcoords.size()) {
		Console::LogWarning ("Texcoord index exceed texcoords count. \
			You are searching for " + std::to_string (position) + " and the size is " +
			std::to_string (_texcoords.size ()));

		return glm::vec2 ();
	}

	return _texcoords [position];
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
	for (auto objModel : _objectModels) {
		for (auto polyGroup : *objModel) {
			for (auto polygon : *polyGroup) {
				if (polygon->VertexCount () > 0 && !polygon->HaveNormals ()) {
					glm::vec3 normal = CalculateNormal (polygon);

					_normals.push_back (normal);

					for (std::size_t k=0;k<polygon->VertexCount ();k++) {
						polygon->AddNormal ((int) _normals.size () - 1);
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

	for (auto objModel : _objectModels) {
		for (auto polyGroup : *objModel) {
			for (auto polygon : *polyGroup) {
				for (std::size_t l=0;l<polygon->VertexCount ();l++) {
					std::size_t vertex = polygon->GetVertex (l);
					std::size_t normal = polygon->GetNormal (l);

					_smoothNormals [vertex] += _normals [normal];
					_smoothNormalsCount [vertex] ++;

					polygon->SetNormal ((int) vertex, l);
				}
			}
		}
	}

	for (std::size_t i=0;i<_smoothNormals.size ();i++) {
		_smoothNormals [i] *= (1.0f / _smoothNormalsCount [i]); 
		_smoothNormals [i] = glm::normalize (_smoothNormals [i]);
	}

	_normals.clear ();
	_normals.shrink_to_fit ();

	/*
	 * Copy all smooth normals to default normals container
	*/

	_normals = _smoothNormals;

	/*
	 * Release smooth normals memory
	*/

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

glm::vec3 Model::CalculateNormal (Polygon* polygon)
{
	glm::vec3 normal = Extensions::VectorExtend::Cross(
		_vertices [polygon->GetVertex (0)],
		_vertices [polygon->GetVertex (1)],
		_vertices [polygon->GetVertex (2)]
	);

	return glm::normalize (normal);
}

BoundingBox* Model::CalculateBoundingBox ()
{
	BoundingBox* boundingBox = new BoundingBox ();

	for (auto objModel : _objectModels) {
		for (auto polyGroup : *objModel) {
			for (auto polygon : *polyGroup) {
				for (std::size_t k=0;k<polygon->VertexCount ();k++) {
					glm::vec3 vertex = _vertices [polygon->GetVertex (k)];

					boundingBox->xmin = std::min (boundingBox->xmin, vertex.x);
					boundingBox->xmax = std::max (boundingBox->xmax, vertex.x);

					boundingBox->ymin = std::min (boundingBox->ymin, vertex.y);
					boundingBox->ymax = std::max (boundingBox->ymax, vertex.y);

					boundingBox->zmin = std::min (boundingBox->zmin, vertex.z);
					boundingBox->zmax = std::max (boundingBox->zmax, vertex.z);
				}
			}
		}
	}

	return boundingBox;	
}
