#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core/Singleton/Singleton.h"

#include "Core/Resources/Resource.h"
#include "Mesh/Model.h"
#include "Material/Material.h"
#include "Texture/Texture.h"
#include "Shader/Shader.h"
#include "Shader/ShaderContent.h"

#include "RenderViews/ModelView.h"
#include "RenderViews/MaterialView.h"
#include "RenderViews/TextureView.h"
#include "RenderViews/ShaderView.h"

#include "Fonts/Font.h"

#include "Renderer/BufferAttribute.h"

struct VertexData
{
	float position[3];
	float normal[3];
	float texcoord[2];

	VertexData ();
};

struct AnimatedVertexData : VertexData
{
	int bones[4];
	float weights[4];

	AnimatedVertexData ();
};

struct NormalMapVertexData : VertexData
{
	float tangent [3];

	NormalMapVertexData ();
};

struct LightMapVertexData : VertexData
{
	float lmTexcoord [2];

	LightMapVertexData ();
};

struct TextGUIVertexData
{
	float position[2];
	float texcoord[2];

	TextGUIVertexData ();
};

class ENGINE_API RenderSystem
{
public:
	static Resource<ModelView> LoadModel (const Resource<Model>& model);
	static Resource<ModelView> LoadAnimationModel (const Resource<Model>& model);
	static Resource<ModelView> LoadNormalMapModel (const Resource<Model>& model);
	static Resource<ModelView> LoadLightMapModel (const Resource<Model>& model);
	static Resource<ModelView> LoadTextGUI (const std::string& text, const Resource<Font>& font);

	static void CreateInstanceModelView (Resource<ModelView>& modelView, const std::vector<BufferAttribute>& attributes, std::size_t size, unsigned char* buffer = nullptr);
	static void UpdateInstanceModelView (Resource<ModelView>& modelView, std::size_t size, std::size_t instancesCount, unsigned char* buffer);

	static Resource<MaterialView> LoadMaterial (const Resource<Material>& material);

	static Resource<TextureView> LoadTexture (const Resource<Texture>& texture);
	static Resource<TextureView> LoadCubeMap (const Resource<Texture>& texture);
	static Resource<TextureView> LoadTextureLUT (const Resource<Texture>& texture);

	static Resource<ShaderView> LoadShader (const Resource<Shader>& shader);
	static Resource<ShaderView> LoadComputeShader (const Resource<Shader>& shader);

	static Resource<Texture> SaveTexture (const Resource<TextureView>& textureView);
private:
	static ObjectBuffer BindModelVertexData (const std::vector<VertexData>& vBuf, const std::vector<unsigned int>& iBuf);
	static ObjectBuffer BindAnimationModelVertexData (const std::vector<AnimatedVertexData>& vBuf, const std::vector<unsigned int>& iBuf);
	static ObjectBuffer BindNormalMapModelVertexData (const std::vector<NormalMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf);
	static ObjectBuffer BindLightMapModelVertexData (const std::vector<LightMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf);

	static glm::vec3 CalculateTangent (const Resource<Model>& model, Polygon* poly);

	static ObjectBuffer ProcessTextGUI (const std::string& text, const Resource<Font>& font);
	static ObjectBuffer BindTextGUIVertexData (const std::vector<TextGUIVertexData>& vBuf, const std::vector<unsigned int>& iBuf);

	static void ProcessMaterial (const Resource<Material>& material, MaterialView* materialView);

	static unsigned int LoadTextureGPU (const Resource<Texture>& texture);
	static unsigned int LoadCubeMapGPU (const Resource<Texture>& texture);
	static unsigned int LoadTextureLUTGPU (const Resource<Texture>& texture);

	static unsigned int BuildShaderContent (const Resource<ShaderContent>& shaderContent, int shaderType);
	static bool ShaderErrorCheck (unsigned int shader);
};

#endif