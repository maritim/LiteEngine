#include "WavefrontObjectLoader.h"

#include <fstream>
#include <string>
#include <limits>

#include "Managers/MaterialManager.h"
#include "Utils/Triangulation/Triangulation.h"

#include "Utils/Files/FileSystem.h"

#include "Mesh/Polygon.h"
#include "Material/MaterialLibrary.h"

#include "Utils/Extensions/StringExtend.h"

#include "Resources.h"

#include "Core/Console/Console.h"

Object* WavefrontObjectLoader::Load(const std::string& filename)
{
	Model * model = new Model ();

	// Add default object models (the syntax can miss)
	ObjectModel* currentObjModel = new ObjectModel ("DEFAULT");
	PolygonGroup* currentPolyGroup = new PolygonGroup ("DEFAULT");

	model->AddObjectModel (currentObjModel);
	currentObjModel->AddPolygonGroup (currentPolyGroup);

	// Initialization part
	std::string currentMatName, lineType;
	bool indexNormalization = false;

	std::ifstream objFile (filename.c_str());

	// Can't open the model. Abort -> TODO: Make the game to not crash if can't load a model.
	if (!objFile.is_open())													// If obj file is open, continue
	{ 
		Console::LogError ("Unable to open file \"" + filename + "\" !");

		exit (1);
	}

	model->SetName (filename);

	// Read data stream
	while (objFile >> lineType)											 	
	{
		if (lineType == "#") {
			ProcessComment(objFile);
		}
		else if (lineType == "mtllib") {
			LoadMaterialLibrary (objFile, filename, model);
		}
		else if (lineType == "v") {
			ReadVertex (objFile, model);
		}
		else if (lineType == "vn") {
			ReadNormal (objFile, model);
		}
		else if (lineType == "vt") {
			ReadTexcoord (objFile, model);
		}
		else if (lineType == "usemtl") {
			ReadCurrentMtlName (objFile, currentMatName);
		}
		else if (lineType == "f") {
			ReadFace (objFile, model, currentPolyGroup, indexNormalization, currentMatName);
		}
		else if (lineType == "o") {
			std::pair<ObjectModel*, PolygonGroup*> result = ReadObject (objFile, model);

			currentObjModel = result.first;
			currentPolyGroup = result.second;
		}
		else if (lineType == "g") {
			currentPolyGroup = ReadPolygonGroup (objFile, currentObjModel);
		} else {
			ProcessComment (objFile);
		}
	}

	objFile.close();

	// Really need this parameter?
	if (indexNormalization) {
		IndexNormalization (model);
	}

	model->GenerateMissingNormals ();
	//model->GenerateSmoothNormals ();

	// major priority TODO: Investigate this
	Triangulation::ConvexTriangulation (model);

	return model;	
}

void WavefrontObjectLoader::ProcessComment(std::ifstream& file)
{
	file.ignore (std::numeric_limits<std::streamsize>::max(), '\n');	
}

void WavefrontObjectLoader::LoadMaterialLibrary(std::ifstream& file, const std::string& filename, Model* model)
{
	std::string mtlfilename;
	std::getline (file, mtlfilename);
	Extensions::StringExtend::Trim (mtlfilename);

	std::string fullMtlFilename = FileSystem::GetDirectory(filename) + mtlfilename;
	fullMtlFilename = FileSystem::FormatFilename (fullMtlFilename);

	Console::Log ("Material name: " + mtlfilename);

	model->SetMaterialLibrary (fullMtlFilename);

	MaterialLibrary* mtlLibrary = Resources::LoadMaterialLibrary(fullMtlFilename);

	for (std::size_t i=0;i<mtlLibrary->GetMaterialsCount ();i++) {
		MaterialManager::Instance ().AddMaterial (mtlLibrary->GetMaterial (i));
	}	
}

void WavefrontObjectLoader::ReadVertex(std::ifstream& file, Model* model)
{
	float x, y, z;
	file >> x >> y >> z;

	model->AddVertex ( new glm::vec3 (x, y, z));
}

void WavefrontObjectLoader::ReadNormal(std::ifstream& file, Model* model)
{
	float x, y, z;
	file >> x >> y >> z;

	model->AddNormal ( new glm::vec3 (x, y, z));	
}

void WavefrontObjectLoader::ReadTexcoord(std::ifstream& file, Model* model)
{
	float x, y;
	file >> x >> y;
	file.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

	model->AddTexcoord ( new glm::vec3 (x, 1.0f-y, 0.0f));
}

void WavefrontObjectLoader::ReadCurrentMtlName(std::ifstream& file, std::string& mtlName)
{
	std::getline (file, mtlName);

	Extensions::StringExtend::Trim (mtlName);
}

std::pair<ObjectModel*, PolygonGroup*> WavefrontObjectLoader::ReadObject (std::ifstream& file, Model* model)
{
	std::string objName;
	std::getline (file, objName);

	ObjectModel* objModel = new ObjectModel (objName);
	model->AddObjectModel (objModel);

	PolygonGroup* polyGroup = new PolygonGroup ("DEFAULT");
	objModel->AddPolygonGroup (polyGroup);

	return std::make_pair (objModel, polyGroup);
}

PolygonGroup* WavefrontObjectLoader::ReadPolygonGroup (std::ifstream& file, ObjectModel* objModel)
{
	std::string polyName;
	std::getline (file, polyName);

	PolygonGroup* polyGroup = new PolygonGroup (polyName);
	objModel->AddPolygonGroup (polyGroup);

	return polyGroup;
}

void WavefrontObjectLoader::ReadFace(std::ifstream& file, Model* model, PolygonGroup* currentPolyGroup, bool& indexNormalization, const std::string& curMatName)
{
	std::string line;

	std::getline(file, line);
	Polygon * face = new Polygon();

	for (std::size_t i=0;i<line.size();i++) 
	{
		if (!isdigit(line[i]) && line[i] != '-') {
			continue;
		}

		int vertexPosition = 0, vertexTexturePosition = 0, vertexNormalPosition = 0;
		int vertexSign = 1, uvSign = 1, normalSign = 1;
		if (line[i] == '-' || isdigit(line[i])) {
			if (line [i] == '-') {
				vertexSign = -1;
				indexNormalization = true;
				i ++;
			}

			for (;isdigit(line[i]);i++) {
				vertexPosition = vertexPosition * 10 + line[i]-'0';
			}

			vertexPosition *= vertexSign;
		}

		if (line[i] == '/') {

			i ++;

			if (line [i] == '-') {
				uvSign = -1;
				indexNormalization = true;
				++ i;
			}

			for (;isdigit(line[i]);i++) {
				vertexTexturePosition = vertexTexturePosition * 10 + line[i] - '0';
			}

			vertexTexturePosition *= uvSign;

			if (line[i] == '/') {
				i++;

				if (line [i] == '-') {
					normalSign = -1;
					indexNormalization = true;
					++ i;
				}

				for (;isdigit(line[i]);i++) {
					vertexNormalPosition = vertexNormalPosition * 10 + line[i] - '0';
				}

				vertexNormalPosition *= normalSign;
			}
		}

		face->AddVertex (vertexPosition-1);

		if (vertexNormalPosition != 0) {
			face->AddNormal (vertexNormalPosition-1);
		}
		
		if (vertexTexturePosition != 0) {
			face->AddTexcoord (vertexTexturePosition-1);
		}
	}

	currentPolyGroup->SetMaterialName (model->GetMaterialLibrary () + "::" + curMatName);

	currentPolyGroup->AddPolygon (face);
}

// If faces are declared by the absolute positions (-1 = latest position,
// -2 = position before and go on), This will normalize the positions
void WavefrontObjectLoader::IndexNormalization (Model* model)
{
	for (std::size_t i=0;i<model->ObjectsCount ();i++) {
		ObjectModel* object = model->GetObject (i);

		for (std::size_t j=0;j<object->GetPolygonCount ();j++) {
			PolygonGroup* polyGroup = object->GetPolygonGroup (j);

			for (std::size_t k=0;k<polyGroup->GetPolygonCount ();k++) {
				Polygon* poly = polyGroup->GetPolygon (k);

				for (std::size_t l=0;l<poly->VertexCount ();l++) {
					int verPos = poly->GetVertex (l);
					if (verPos < 0) {
						poly->SetVertex (model->VertexCount () + verPos + 1, l);
					}

					if (poly->HaveNormals ()) {
						int norPos = poly->GetNormal (l);
						if (norPos < 0) {
							poly->SetNormal (model->NormalsCount () + norPos + 1, l);
						}
					}

					if (poly->HaveUV ()) {
						int texPos = poly->GetTexcoord (l);
						if (texPos < 0) {
							poly->SetTexcoord (model->TexcoordsCount () + texPos + 1, l);
						}
					}
				}
			}
		}
	}
}
