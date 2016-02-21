#ifndef WAVEFRONTOBJECTLOADER_H
#define WAVEFRONTOBJECTLOADER_H

#include "ResourceLoader.h"

#include <string>

#include "Mesh/Model.h"
#include "Mesh/PolygonGroup.h"
#include "Mesh/ObjectModel.h"

class WavefrontObjectLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& fileName);
private:
	void ProcessComment(std::ifstream& file);
	void LoadMaterialLibrary(std::ifstream& file, const std::string& filename, Model* model);
	void ReadVertex(std::ifstream& file, Model* model);
	void ReadNormal(std::ifstream& file, Model* model);
	void ReadTexcoord(std::ifstream& file, Model* model);
	void ReadCurrentMtlName(std::ifstream& file, std::string& mtlName);
	void ReadFace(std::ifstream& file, Model* model, PolygonGroup* polyGroup, bool& indexNorm, const std::string& curMatName);
	std::pair<ObjectModel*, PolygonGroup*> ReadObject(std::ifstream& file, Model* model);
	PolygonGroup* ReadPolygonGroup(std::ifstream& file, ObjectModel* objModel);
	void IndexNormalization(Model* model);
};

#endif