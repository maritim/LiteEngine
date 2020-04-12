#include "Resources.h"

#include "Core/Console/Console.h"

#include "Utils/Files/FileSystem.h"

/*
 * Load
*/

#include "Loaders/SettingsLoader.h"
#include "Loaders/RenderSettingsLoader.h"
#include "Loaders/WavefrontObjectLoader.h"
#include "Loaders/StanfordObjectLoader.h"
#include "Loaders/GenericObjectModelLoader.h"
#include "Loaders/AnimationModelLoader.h"
#include "Loaders/AnimationSkinLoader.h"
#include "Loaders/AnimationClipLoader.h"
#include "Loaders/WAVLoader.h"
#include "Loaders/ShaderLoader.h"
#include "Loaders/ComputeShaderLoader.h"
#include "Loaders/ShaderContentLoader.h"
#include "Loaders/MaterialLibraryLoader.h"
#include "Loaders/TextureLoader.h"
#include "Loaders/TextureAtlasLoader.h"
#include "Loaders/CubeMapLoader.h"
#include "Loaders/ParticleSystemLoader.h"
#include "Loaders/SkyboxLoader.h"
#include "Loaders/BitmapFontLoader.h"

/*
 * Save
*/

#include "Savers/PNGSaver.h"

/*
 * Load
*/

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

Resource<AudioClip> Resources::LoadAudioClip (const std::string& filename)
{
	if (Resource<AudioClip>::GetResource (filename) != nullptr) {
		return Resource<AudioClip>::GetResource (filename);
	}

	AudioClip* audioClip = nullptr;

	std::string extension = FileSystem::GetExtension(filename);

	if (extension == ".wav") {
		audioClip = LoadWAV (filename);
	}

	Console::LogError ("The File Format for " + extension + " is not recognized.");

	return Resource<AudioClip> (audioClip, filename);
}

AudioClip* Resources::LoadWAV (const std::string& filename)
{
	WAVLoader* wavLoader = new WAVLoader ();

	AudioClip* audioClip = (AudioClip*)wavLoader->Load (filename);

	delete wavLoader;

	return audioClip;
}

Resource<Shader> Resources::LoadShader (const std::vector<std::string>& filenames)
{
	std::string filename = filenames [0] + filenames [1] + (filenames.size () > 2 ? filenames [2] : "");

	if (Resource<Shader>::GetResource (filename) != nullptr) {
		return Resource<Shader>::GetResource (filename);
	}

	ShaderLoader* shaderLoader = new ShaderLoader ();

	shaderLoader->SetFilenames (filenames);

	Shader* shader = (Shader*)shaderLoader->Load (filename);

	delete shaderLoader;

	return Resource<Shader> (shader, filename);
}

Resource<Shader> Resources::LoadComputeShader (const std::string& filename)
{
	if (Resource<Shader>::GetResource (filename) != nullptr) {
		return Resource<Shader>::GetResource (filename);
	}

	ComputeShaderLoader* computeShaderLoader = new ComputeShaderLoader ();

	Shader* shader = (Shader*)computeShaderLoader->Load (filename);

	delete computeShaderLoader;

	return Resource<Shader> (shader, filename);
}

Resource<ShaderContent> Resources::LoadShaderContent (const std::string& filename)
{
	if (Resource<ShaderContent>::GetResource (filename) != nullptr) {
		return Resource<ShaderContent>::GetResource (filename);
	}

	ShaderContentLoader* shaderContentLoader = new ShaderContentLoader ();

	ShaderContent* shaderContent = (ShaderContent*)shaderContentLoader->Load (filename);

	delete shaderContentLoader;

	return Resource<ShaderContent> (shaderContent, filename);
}

Resource<Texture> Resources::LoadTexture(const std::string& filename)
{
	if (Resource<Texture>::GetResource (filename) != nullptr) {
		return Resource<Texture>::GetResource (filename);
	}

	TextureLoader* textureLoader = new TextureLoader();

	Texture* texture = (Texture*) textureLoader->Load(filename);

	delete textureLoader;

	return Resource<Texture> (texture, filename);
}

Resource<Texture> Resources::LoadCubemap (const std::vector<std::string>& filename)
{
	if (Resource<Texture>::GetResource (filename [0]) != nullptr) {
		return Resource<Texture>::GetResource (filename [0]);
	}

	CubeMapLoader* cubeMapLoader = new CubeMapLoader();
	cubeMapLoader->SetFilenames (filename);

	CubeMap* cubeMap = (CubeMap*) cubeMapLoader->Load(filename[0]);

	delete cubeMapLoader;

	return Resource<Texture> (cubeMap, filename [0]);
}

Resource<Texture> Resources::LoadTextureAtlas (const std::string& filename)
{
	if (Resource<Texture>::GetResource (filename) != nullptr) {
		return Resource<Texture>::GetResource (filename);
	}

	TextureAtlasLoader* texAtlasLoader = new TextureAtlasLoader ();

	TextureAtlas* texAtlas = (TextureAtlas*) texAtlasLoader->Load (filename);

	delete texAtlasLoader;

	return Resource<Texture> (texAtlas, filename);
}

Resource<Font> Resources::LoadBitmapFont (const std::string& filename)
{
	if (Resource<Font>::GetResource (filename) != nullptr) {
		return Resource<Font>::GetResource (filename);
	}

	BitmapFontLoader* bitmapFontLoader = new BitmapFontLoader ();

	BitmapFont* bitmapFont = (BitmapFont*) bitmapFontLoader->Load (filename);

	delete bitmapFontLoader;

	return Resource<Font> (bitmapFont, filename);
}

Resource<MaterialLibrary> Resources::LoadMaterialLibrary(const std::string& filename)
{
	if (Resource<MaterialLibrary>::GetResource (filename) != nullptr) {
		return Resource<MaterialLibrary>::GetResource (filename);
	}

	MaterialLibraryLoader* materialLibraryLoader = new MaterialLibraryLoader();

	MaterialLibrary* materialLibrary = (MaterialLibrary*) materialLibraryLoader->Load(filename);

	delete materialLibraryLoader;

	return Resource<MaterialLibrary> (materialLibrary, filename);
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

/*
 * Save
*/

bool Resources::SaveTexture (const Resource<Texture>& texture, const std::string& filename)
{
	std::string extension = FileSystem::GetExtension (filename);

	if (extension == ".png") {
		return SavePNG (texture, filename);
	}

	return false;
}

bool Resources::SavePNG (const Resource<Texture>& texture, const std::string& filename)
{
	PNGSaver* pngSaver = new PNGSaver ();

	bool saveResult = pngSaver->Save (&*texture, filename);

	delete pngSaver;

	return saveResult;
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