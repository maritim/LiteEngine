#ifndef WAVEFRONTOBJECTLOADER_H
#define WAVEFRONTOBJECTLOADER_H

#include "Resources/ResourceLoader.h"

#include <string>

#include "Renderer/Render/Mesh/Model.h"
#include "Renderer/Render/Mesh/PolygonGroup.h"
#include "Renderer/Render/Mesh/ObjectModel.h"

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Material/MaterialLibrary.h"
#include "Renderer/Render/Material/Material.h"

class WavefrontObjectLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& fileName);
private:
	void ProcessComment(std::ifstream& file);
	void LoadMaterialLibrary(std::ifstream& file, Resource<MaterialLibrary>& matLibrary, const std::string& filename, Model* model);
	void ReadVertex(std::ifstream& file, Model* model);
	void ReadNormal(std::ifstream& file, Model* model);
	void ReadTexcoord(std::ifstream& file, Model* model);
	void ReadCurrentMtlName(std::ifstream& file, Resource<Material>& curMat, const Resource<MaterialLibrary>& currentMatLibrary);
	void ReadFace(std::ifstream& file, Model* model, PolygonGroup* polyGroup, bool& indexNorm, const Resource<Material>& curMat);
	std::pair<ObjectModel*, PolygonGroup*> ReadObject(std::ifstream& file, Model* model);
	PolygonGroup* ReadPolygonGroup(std::ifstream& file, ObjectModel* objModel);
};

#endif