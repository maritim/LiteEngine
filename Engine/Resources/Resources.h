#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>
#include <string>

#include "Core/Settings/SettingsContainer.h"
#include "Mesh/Model.h"
#include "Mesh/AnimationModel.h"
#include "Mesh/AnimationContainer.h"
#include "Material/MaterialLibrary.h"
#include "Texture/Texture.h"
#include "Texture/TextureAtlas.h"
#include "Texture/CubeMap.h"
#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "Skybox/Skybox.h"
#include "Fonts/BitmapFont.h"
#include "Lighting/Light.h"

class Resources
{
public:
	static SettingsContainer* LoadSettings (const std::string& filename);

	static Model* LoadModel (const std::string& filename);
	static AnimationModel* LoadAnimatedModel (const std::string& filename);
//	static int SaveModel (Model* model, char* filename);

	static AnimationModel* LoadSkinModel (const std::string& filename);
	static AnimationContainer* LoadAnimationClip (const std::string& filename);
	
	static Texture* LoadTexture (const std::string& filename);
	static TextureAtlas* LoadTextureAtlas (const std::string& filename);
	static CubeMap* LoadCubemap (const std::vector<std::string>& filenames);
//	static unsigned int Load_R5G6B5_BMP (const std::string filename);

	static BitmapFont* LoadBitmapFont (const std::string& filename); 

	static MaterialLibrary* LoadMaterialLibrary (const std::string& filename);
	static ParticleSystem* LoadParticleSystem (const std::string& filename);
	static Skybox* LoadSkybox (const std::string& filename);
	static BulletCollider* LoadCollider (const std::string& filename);
	static Light* LoadLight (const std::string& filename);

private:
	static Model* LoadWavefrontModel (const std::string& filename);
	static Model* LoadStanfordModel (const std::string& filename);
	static Model* LoadGenericModel (const std::string& filename);

//	static int SaveWavefrontModel (Model* model, char* filename);
//	static int SaveStanfordModel (Model* model, char* filename);
};

#endif