#include "Resources.h"

#include "Core/Console/Console.h"

#include "Utils/Files/FileSystem.h"

#include "SettingsLoader.h"
#include "RenderSettingsLoader.h"
#include "WavefrontObjectLoader.h"
#include "StanfordObjectLoader.h"
#include "GenericObjectModelLoader.h"
#include "AnimationModelLoader.h"
#include "AnimationSkinLoader.h"
#include "AnimationClipLoader.h"
#include "WAVLoader.h"
#include "ShaderContentLoader.h"
#include "MaterialLibraryLoader.h"
#include "TextureLoader.h"
#include "TextureAtlasLoader.h"
#include "CubeMapLoader.h"
#include "TextureLUTLoader.h"
#include "ParticleSystemLoader.h"
#include "SkyboxLoader.h"
#include "BitmapFontLoader.h"
#include "ColliderLoader.h"
#include "LightLoader.h"

SettingsContainer* Resources::LoadSettings (const std::string& filename)
{
	SettingsLoader* settingsLoader = new SettingsLoader ();

	SettingsContainer* settingsContainer = (SettingsContainer*)settingsLoader->Load (filename);

	delete settingsLoader;

	return settingsContainer;
}

RenderSettings* Resources::LoadRenderSettings (const std::string& filename)
{
	RenderSettingsLoader* settingsLoader = new RenderSettingsLoader ();

	RenderSettings* settings = (RenderSettings*)settingsLoader->Load (filename);

	delete settingsLoader;

	return settings;
}

Resource<Model> Resources::LoadModel (const std::string& filename)
{
	std::string extension = FileSystem::GetExtension(filename);

	if (Resource<Model>::GetResource (filename) != nullptr) {
		return Resource<Model>::GetResource (filename);
	}

	Model* mesh = nullptr;

	if (extension == ".obj") {
		mesh = LoadWavefrontModel (filename);
	}
	else if (extension == ".ply") {
		mesh = LoadStanfordModel (filename);
	} else {
		mesh = LoadGenericModel (filename);
	}

	Console::LogError ("The File Format for " + extension + " is not recognized.");

	return Resource<Model> (mesh, filename);
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

Resource<Model> Resources::LoadAnimatedModel (const std::string& filename)
{
	if (Resource<Model>::GetResource (filename) != nullptr) {
		return Resource<Model>::GetResource (filename);
	}

	AnimationModelLoader* animatedModelLoader = new AnimationModelLoader ();

	AnimationModel* animatedModel = (AnimationModel*)animatedModelLoader->Load (filename);

	delete animatedModelLoader;

	return Resource<Model> (animatedModel, filename);
}

AnimationModel* Resources::LoadSkinModel (const std::string& filename)
{
	AnimationSkinLoader* animatedSkinLoader = new AnimationSkinLoader ();

	AnimationModel* animatedModel = (AnimationModel*)animatedSkinLoader->Load (filename);

	delete animatedSkinLoader;

	return animatedModel;
}

AnimationContainer* Resources::LoadAnimationClip (const std::string& filename)
{
	AnimationClipLoader* animationClipLoader = new AnimationClipLoader ();

	AnimationContainer* animContainer = (AnimationContainer*)animationClipLoader->Load (filename);

	delete animationClipLoader;

	return animContainer;
}

AudioClip* Resources::LoadAudioClip (const std::string& filename)
{
	std::string extension = FileSystem::GetExtension(filename);

	if (extension == ".wav") {
		return LoadWAV (filename);
	}

	Console::LogError ("The File Format for " + extension + " is not recognized.");

	return nullptr;
}

AudioClip* Resources::LoadWAV (const std::string& filename)
{
	WAVLoader* wavLoader = new WAVLoader ();

	AudioClip* audioClip = (AudioClip*)wavLoader->Load (filename);

	delete wavLoader;

	return audioClip;
}

ShaderContent* Resources::LoadShaderContent (const std::string& filename)
{
	ShaderContentLoader* shaderContentLoader = new ShaderContentLoader ();

	ShaderContent* shaderContent = (ShaderContent*)shaderContentLoader->Load (filename);

	delete shaderContentLoader;

	return shaderContent;
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

TextureLUT* Resources::LoadTextureLUT (const std::string& filename)
{
	TextureLUTLoader* textureLUTLoader = new TextureLUTLoader ();

	TextureLUT* lutTexture = (TextureLUT*) textureLUTLoader->Load (filename);

	delete textureLUTLoader;

	return lutTexture;
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