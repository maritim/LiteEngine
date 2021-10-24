#include "StanfordObjectLoader.h"

#include <string>
#include <iostream>
#include <limits>
#include <fstream>

#include "Renderer/Render/Mesh/Model.h"

// Deprecated
Object* StanfordObjectLoader::Load(const std::string& filename)
{
	Model* model = new Model ();

	return model;

	// std::string keyWord;
	// std::string typeVar;
	// std::ifstream objFile (filename.c_str());	

	// std::size_t vertexCount = 0;
	// std::size_t faceCount = 0;

	// typedef enum {PROPERTY_X = 0, PROPERTY_Y, PROPERTY_Z, PROPERTY_RED, PROPERTY_GREEN, PROPETRY_BLUE,
	// 			  PROPERTY_NX, PROPERTY_NY, PROPERTY_NZ, PROPERTY_INTENSITY, PROPERTY_CONFIDENCE} property_t;
	
	// if (objFile.is_open())													// If obj file is open, continue
	// { 
	// 	model->SetName (filename);
	// 	// Read the header of file

	// 	int propertyOrder [11];
	// 	bool propertyExistence [11];
	// 	float properties [11];

	// 	int propertyCount = 0;

	// 	do
	// 	{
	// 		objFile >> keyWord;

	// 		if (keyWord == "format") {
	// 			objFile >> typeVar;
	// 			if (typeVar != "ascii") {
	// 				throw "Another format for .ply Stanford than ASCII is not supported yet!";
	// 			}
	// 		}
	// 		if (keyWord == "comment" || keyWord == "ply") {
	// 			objFile.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
	// 		}
	// 		if (keyWord == "element") {
	// 			objFile >> typeVar;
	// 			if (typeVar == "vertex") {
	// 				objFile >> vertexCount;
	// 			}
	// 			if (typeVar == "face") {
	// 				objFile >> faceCount;
	// 			}
	// 		}
	// 		if (keyWord == "property") {
	// 			objFile >> typeVar;
	// 			if (typeVar == "float") {
	// 				objFile >> typeVar;
	// 			}
	// 			// Is a property for faces...
	// 			// For the moment we support only n v_index v_index v_index format
	// 			if (typeVar == "list") {
	// 				objFile.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
	// 				continue;
	// 			}

	// 			if (typeVar == "x") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_X;
	// 			}
	// 			if (typeVar == "y") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_Y;
	// 			}
	// 			if (typeVar == "z") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_Z;
	// 			}
	// 			if (typeVar == "red") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_RED;
	// 			}
	// 			if (typeVar == "green") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_GREEN;
	// 			}
	// 			if (typeVar == "blue") {
	// 				propertyOrder [propertyCount ++] = PROPETRY_BLUE;
	// 			}
	// 			if (typeVar == "nx") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_NX;
	// 			}
	// 			if (typeVar == "ny") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_NY;
	// 			}
	// 			if (typeVar == "nz") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_NZ;
	// 			}
	// 			if (typeVar == "intensity") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_INTENSITY;
	// 			}
	// 			if (typeVar == "confidence") {
	// 				propertyOrder [propertyCount ++] = PROPERTY_CONFIDENCE;
	// 			}
	// 		}

	// 	} while (keyWord != "end_header");

	// 	for(std::size_t i=0;i<vertexCount;i++) {
	// 		for (std::size_t j=0;j<propertyCount;j++) {
	// 			objFile >> properties [propertyOrder [j]];
	// 		}

	// 		model->AddVertex ( new Vector3 (properties [PROPERTY_X], properties [PROPERTY_Y],
	// 			properties [PROPERTY_Z]));
	// 	}

	// 	for (std::size_t i=0;i<faceCount;i++) {
	// 		std::size_t vertCount;
	// 		int vertPos;
	// 		objFile >> 	vertCount;

	// 		Polygon* face = new Polygon ();
	// 		for (std::size_t j=0;j<vertCount;j++) {
	// 			objFile >> vertPos;

	// 			face->AddVertex (vertPos);
	// 		}

	// 		model->AddFace (face);
	// 	}

	// 	objFile.close();														// Close OBJ file
	// }
	// else 
	// {
	// 	std::cout << "Unable to open file";					

	// 	return NULL;			
	// }

	// model->GenerateSmoothNormals ();

	// return model;
}