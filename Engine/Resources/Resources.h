#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>
#include <string>

#include "Core/Resources/Resource.h"

#include "Core/Settings/SettingsContainer.h"
#include "Renderer/RenderSettings.h"
#include "Renderer/Render/Mesh/Model.h"
#include "Renderer/Render/Mesh/AnimationModel.h"
#include "Renderer/Render/Mesh/AnimationContainer.h"
#include "Audio/AudioClip.h"
#include "Renderer/Render/Shader/Shader.h"
#include "Renderer/Render/Shader/ShaderContent.h"
#include "Renderer/Render/Material/MaterialLibrary.h"
#include "Renderer/Render/Texture/Texture.h"
#include "Renderer/Render/Texture/TextureAtlas.h"
#include "Renderer/Render/Texture/CubeMap.h"
#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "Skybox/Skybox.h"
#include "Renderer/Render/Fonts/BitmapFont.h"

class ENGINE_API Resources
{
public:
	/*
	 * Load
	*/

	static SettingsContainer* LoadSettings (const std::string& filename);
	static RenderSettings* LoadRenderSettings (const std::string& filename);

	static Resource<Model> LoadModel (const std::string& filename);
	static Resource<Model> LoadAnimatedModel (const std::string& filename);

	static AnimationModel* LoadSkinModel (const std::string& filename);
	static AnimationContainer* LoadAnimationClip (const std::string& filename);

	static Resource<AudioClip> LoadAudioClip (const std::string& filename);

	static Resource<Shader> LoadShader (const std::vector<std::string>& filenames);
	static Resource<Shader> LoadComputeShader (const std::string& filename);
	static Resource<ShaderContent> LoadShaderContent (const std::string& filename);
	
	static Resource<Texture> LoadTexture (const std::string& filename);
	static Resource<Texture> LoadTextureAtlas (const std::string& filename);
	static Resource<Texture> LoadCubemap (const std::vector<std::string>& filenames);

	static Resource<Font> LoadBitmapFont (const std::string& filename); 

	static Resource<MaterialLibrary> LoadMaterialLibrary (const std::string& filename);
	static ParticleSystem* LoadParticleSystem (const std::string& filename);
	static Skybox* LoadSkybox (const std::string& filename);

	/*
	 * Save
	*/

	static bool SaveTexture (const Resource<Texture>& texture, const std::string& filename);

private:
	/*
	 * Load
	*/

	static Model* LoadWavefrontModel (const std::string& filename);
	static Model* LoadStanfordModel (const std::string& filename);
	static Model* LoadGenericModel (const std::string& filename);

	static AudioClip* LoadWAV (const std::string& filename);

	/*
	 * Save
	*/

	static bool SavePNG (const Resource<Texture>& texture, const std::string& filename);

//	static int SaveWavefrontModel (Model* model, char* filename);
//	static int SaveStanfordModel (Model* model, char* filename);
};

#endif