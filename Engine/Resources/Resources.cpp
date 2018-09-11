#include "Resources.h"

#include "Core/Console/Console.h"

#include "Utils/Files/FileSystem.h"

#include "WavefrontObjectLoader.h"
#include "StanfordObjectLoader.h"
#include "GenericObjectModelLoader.h"
#include "AnimationModelLoader.h"
#include "MaterialLibraryLoader.h"
#include "TextureLoader.h"
#include "TextureAtlasLoader.h"
#include "CubeMapLoader.h"
#include "ParticleSystemLoader.h"
#include "SkyboxLoader.h"
#include "BitmapFontLoader.h"
#include "ColliderLoader.h"
#include "LightLoader.h"

Model* Resources::LoadModel (const std::string& filename)
{
	std::string extension = FileSystem::GetExtension(filename);

	if (extension == ".obj") {
		return LoadWavefrontModel (filename);
	}
	else if (extension == ".ply") {
		return LoadStanfordModel (filename);
	} else {
		return LoadGenericModel (filename);
	}

	Console::LogError ("The File Format for " + extension + " is not recognized.");

	return NULL;
}

Model* Resources::LoadWavefrontModel(const std::string& filename)
{
	WavefrontObjectLoader* wavefrontObjectLoader = new WavefrontObjectLoader();

	Model* model = (Model*)wavefrontObjectLoader->Load(filename);

	delete wavefrontObjectLoader;

	return model;
}

Model* Resources::LoadStanfordModel (const std::string& filename)
{
	StanfordObjectLoader* stanfordObjectLoader = new StanfordObjectLoader();

	Model* model = (Model*)stanfordObjectLoader->Load(filename);

	delete stanfordObjectLoader;

	return model;
}

Model* Resources::LoadGenericModel (const std::string& filename)
{
	GenericObjectModelLoader* genericModelLoader = new GenericObjectModelLoader ();

	Model* model = (Model*)genericModelLoader->Load (filename);

	delete genericModelLoader;

	return model;
}

AnimationModel* Resources::LoadAnimatedModel (const std::string& filename)
{
	AnimationModelLoader* animatedModelLoader = new AnimationModelLoader ();

	AnimationModel* animatedModel = (AnimationModel*)animatedModelLoader->Load (filename);

	delete animatedModelLoader;

	return animatedModel;
}

Texture* Resources::LoadTexture(const std::string& filename)
{
	TextureLoader* textureLoader = new TextureLoader();

	Texture* texture = (Texture*) textureLoader->Load(filename);

	delete textureLoader;

	return texture;
}

CubeMap* Resources::LoadCubemap (const std::vector<std::string>& filename)
{
	CubeMapLoader* cubeMapLoader = new CubeMapLoader();
	cubeMapLoader->SetFilenames (filename);

	CubeMap* cubeMap = (CubeMap*) cubeMapLoader->Load(filename[0]);

	delete cubeMapLoader;

	return cubeMap;
}

TextureAtlas* Resources::LoadTextureAtlas (const std::string& filename)
{
	TextureAtlasLoader* texAtlasLoader = new TextureAtlasLoader ();

	TextureAtlas* texAtlas = (TextureAtlas*) texAtlasLoader->Load (filename);

	delete texAtlasLoader;

	return texAtlas;
}

BitmapFont* Resources::LoadBitmapFont (const std::string& filename)
{
	BitmapFontLoader* bitmapFontLoader = new BitmapFontLoader ();

	BitmapFont* bitmapFont = (BitmapFont*) bitmapFontLoader->Load (filename);

	delete bitmapFontLoader;

	return bitmapFont;
}

MaterialLibrary* Resources::LoadMaterialLibrary(const std::string& filename)
{
	MaterialLibraryLoader* materialLibraryLoader = new MaterialLibraryLoader();

	MaterialLibrary* materialLibrary = (MaterialLibrary*) materialLibraryLoader->Load(filename);

	delete materialLibraryLoader;

	return materialLibrary;
}

ParticleSystem* Resources::LoadParticleSystem(const std::string& filename)
{
	ParticleSystemLoader* particleSystemLoader = new ParticleSystemLoader ();

	ParticleSystem* particleSystem = (ParticleSystem*) particleSystemLoader->Load (filename);

	delete particleSystemLoader;

	return particleSystem;
}

Skybox* Resources::LoadSkybox (const std::string& filename)
{
	SkyboxLoader* skyboxLoader = new SkyboxLoader ();

	Skybox* skybox = (Skybox*) skyboxLoader->Load (filename);

	delete skyboxLoader;

	return skybox;
}

BulletCollider* Resources::LoadCollider (const std::string& filename)
{
	ColliderLoader* colliderLoader = new ColliderLoader ();

	BulletCollider* collider = (BulletCollider*) colliderLoader->Load (filename);

	delete colliderLoader;

	return collider;
}

Light* Resources::LoadLight (const std::string& filename)
{
	LightLoader* lightLoader = new LightLoader ();

	Light* light = (Light*) lightLoader->Load (filename);

	delete lightLoader;

	return light;
}

// bool SortingMethod (Polygon* a, Polygon* b) { return (a->matName < b->matName); }

// int Resources::SaveModel(Model * model, char *filename)
// {
// 	std::ofstream objFile (filename);	
// 	if (objFile.is_open())													// If obj file is open, continue
// 	{
// 		if (model->GetMaterialLibrary () != "") {
// 			std::string mtllibName = model->GetMaterialLibrary ();
// 			objFile << "mtllib " << mtllibName << "\n";
// 		}

// 		for (std::size_t i=0;i<model->VertexCount ();i++) {
// 			objFile << "v " << *model->GetVertex (i) << "\n";
// 		}
// 		objFile << "# " << model->VertexCount () << " vertices generated\n\n";

// 		if (model->HaveUV ()) {
// 			for (std::size_t i=0;i<model->TexcoordsCount ();i++) {
// 				objFile << "vt " << model->GetTexcoord (i)->x << " " << model->GetTexcoord (i)->y << "\n";
// 			}
// 			objFile << "# " << model->TexcoordsCount () << " texcoords generated\n\n";
// 		}

// 		for (std::size_t i=0;i<model->NormalsCount ();i++) {
// 			objFile << "vn " << *model->GetNormal (i) << "\n";
// 		}
// 		objFile << "# " << model->NormalsCount () << " normals generated\n\n";

// 		std::vector<Polygon*> faces;

// 		for (std::size_t i=0;i<model->FacesCount ();i++) {
// 			faces.push_back (model->GetFace (i));
// 		}

// 		std::sort (faces.begin(), faces.end(), SortingMethod);

// 		std::string currentMaterial = "";
// 		for (std::size_t i=0;i<faces.size();i++) {
// 			if (faces[i]->matName != currentMaterial) {
// 				objFile << "usemtl " << faces[i]->matName << "\n";
// 				currentMaterial = faces[i]->matName;
// 			}

// 			objFile << "f ";
// 			for (std::size_t j=0;j<faces[i]->VertexCount ();j++) {
// 				if (model->HaveUV ()) {
// 					objFile << faces[i]->GetVertex (j) + 1 << "/" << faces[i]->GetTexcoord (j) + 1 << "/" << faces[i]->GetNormal (j) + 1 << " ";
// 				} else {
// 					objFile << faces[i]->GetVertex (j) + 1 << "//" << faces[i]->GetNormal (j) + 1 << " ";
// 				}
// 			}
// 			objFile << "\n";
// 		}
// 		objFile << "# " << faces.size () << " faces generated\n";

// 		objFile.close ();
// 	}

// 	return 1;
// }