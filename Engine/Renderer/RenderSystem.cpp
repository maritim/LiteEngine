#include "RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Mesh/AnimationModel.h"
#include "Mesh/LightMapModel.h"

#include "Texture/CubeMap.h"

#include "Shader/DrawingShader.h"
#include "Shader/ComputeShader.h"

#include "Renderer/RenderViews/CubeMapView.h"
#include "RenderViews/TextureLUTView.h"

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Extensions/MathExtend.h"

VertexData::VertexData ()
{
	for (std::size_t i=0;i<3;i++) {
		position [i] = normal [i] = 0;
	}

	texcoord [0] = texcoord [1] = 0;
}

AnimatedVertexData::AnimatedVertexData () : VertexData ()
{
	for (std::size_t i=0;i<4;i++) {
		bones [i] = 0;
		weights [i] = 0;
	}
}

NormalMapVertexData::NormalMapVertexData () : VertexData ()
{
	for (std::size_t i = 0; i < 3; i++) {
		tangent [i] = 0;
	}
}

LightMapVertexData::LightMapVertexData () : VertexData ()
{
	for (std::size_t i = 0; i < 2; i++) {
		lmTexcoord [i] = 0;
	}
}

TextGUIVertexData::TextGUIVertexData ()
{
	for (std::size_t i = 0; i < 2; i++) {
		position [i] = texcoord [i] = 0;
	}
}

// http://szudzik.com/ElegantPairing.pdf
std::size_t hash (std::size_t a, std::size_t b)
{
	return a >= b ? a * a + a + b : a + b * b;
}

Resource<ModelView> RenderSystem::LoadModel (const Resource<Model>& model)
{
	if (Resource<ModelView>::GetResource (model->GetName ()) != nullptr) {
		return Resource<ModelView>::GetResource (model->GetName ());
	}

	ModelView* modelView = new ModelView ();

	/*
	 * Create buffer data
	*/

	std::map<std::size_t, std::size_t> indices;

	std::vector<VertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			std::size_t startIndex = indexBuffer.size ();

			for (Polygon* polygon : *polyGroup) {
				for(std::size_t j=0;j<polygon->VertexCount();j++) {

					std::size_t index = 0;

					std::size_t vertexPos = polygon->GetVertex (j);
					std::size_t normalPos = polygon->GetNormal (j);
					std::size_t texcoordPos = polygon->GetTexcoord (j);

					std::size_t hashPos = hash (vertexPos, hash (normalPos, texcoordPos));

					auto indexIt = indices.find (hashPos);

					if (indexIt == indices.end ()) {
						VertexData vertexData;

						glm::vec3 position = model->GetVertex (polygon->GetVertex(j));
						vertexData.position[0] = position.x;
						vertexData.position[1] = position.y;
						vertexData.position[2] = position.z;

						if (polygon->HaveNormals ()) {
							glm::vec3 normal = model->GetNormal (polygon->GetNormal(j));
							vertexData.normal[0] = normal.x;
							vertexData.normal[1] = normal.y;
							vertexData.normal[2] = normal.z;
						}

						if (model->HaveUV()) {
							glm::vec2 texcoord = model->GetTexcoord (polygon->GetTexcoord(j));
							vertexData.texcoord[0] = texcoord.x;
							vertexData.texcoord[1] = texcoord.y;
						}

						vertexBuffer.push_back (vertexData);

						index = vertexBuffer.size () - 1;

						indices [hashPos] = index;
					} else {
						index = indexIt->second;
					}

					indexBuffer.push_back (index);
				}
			}

			GroupBuffer groupBuffer;

			groupBuffer.materialView = LoadMaterial (polyGroup->GetMaterial ());
			groupBuffer.offset = startIndex;
			groupBuffer.INDEX_COUNT = indexBuffer.size () - startIndex;

			modelView->AddGroupBuffer (groupBuffer);
		}
	}

	ObjectBuffer objectBuffer = BindModelVertexData (vertexBuffer, indexBuffer);

	modelView->SetObjectBuffer (objectBuffer);

	return Resource<ModelView> (modelView, model->GetName ());
}

Resource<ModelView> RenderSystem::LoadAnimationModel (const Resource<Model>& model)
{
	if (Resource<ModelView>::GetResource (model->GetName ()) != nullptr) {
		return Resource<ModelView>::GetResource (model->GetName ());
	}

	ModelView* modelView = new ModelView ();

	/*
	 * Create buffer data
	*/

	const AnimationModel* animModel = dynamic_cast<const AnimationModel*> (&*model);

	std::map<std::size_t, std::size_t> indices;

	std::vector<AnimatedVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			std::size_t startIndex = indexBuffer.size ();

			for (Polygon* polygon : *polyGroup) {
				for(std::size_t j=0;j<polygon->VertexCount();j++) {

					std::size_t index = 0;

					std::size_t vertexPos = polygon->GetVertex (j);
					std::size_t normalPos = polygon->GetNormal (j);
					std::size_t texcoordPos = polygon->GetTexcoord (j);

					std::size_t hashPos = hash (vertexPos, hash (normalPos, texcoordPos));

					auto indexIt = indices.find (hashPos);

					if (indexIt == indices.end ()) {
						AnimatedVertexData vertexData;

						glm::vec3 position = animModel->GetVertex (polygon->GetVertex(j));
						vertexData.position[0] = position.x;
						vertexData.position[1] = position.y;
						vertexData.position[2] = position.z;

						if (polygon->HaveNormals ()) {
							glm::vec3 normal = animModel->GetNormal (polygon->GetNormal(j));
							vertexData.normal[0] = normal.x;
							vertexData.normal[1] = normal.y;
							vertexData.normal[2] = normal.z;
						}

						if (animModel->HaveUV()) {
							glm::vec2 texcoord = animModel->GetTexcoord (polygon->GetTexcoord(j));
							vertexData.texcoord[0] = texcoord.x;
							vertexData.texcoord[1] = texcoord.y;
						}

						VertexBoneInfo* vertexBoneInfo = animModel->GetVertexBoneInfo (polygon->GetVertex (j));

						for (std::size_t k=0;k<4 && k<vertexBoneInfo->GetBoneIDsCount ();k++) {
							vertexData.bones [k] = vertexBoneInfo->GetBoneID (k);
							vertexData.weights [k] = vertexBoneInfo->GetBoneWeight (k);
						}

						vertexBuffer.push_back (vertexData);

						index = vertexBuffer.size () - 1;

						indices [hashPos] = index;
					} else {
						index = indexIt->second;
					}

					indexBuffer.push_back (index);
				}
			}

			GroupBuffer groupBuffer;

			groupBuffer.materialView = LoadMaterial (polyGroup->GetMaterial ());
			groupBuffer.offset = startIndex;
			groupBuffer.INDEX_COUNT = indexBuffer.size () - startIndex;

			modelView->AddGroupBuffer (groupBuffer);
		}
	}

	ObjectBuffer objectBuffer = BindAnimationModelVertexData (vertexBuffer, indexBuffer);

	modelView->SetObjectBuffer (objectBuffer);

	return Resource<ModelView> (modelView, model->GetName ());
}

Resource<ModelView> RenderSystem::LoadNormalMapModel (const Resource<Model>& model)
{
	if (Resource<ModelView>::GetResource (model->GetName ()) != nullptr) {
		return Resource<ModelView>::GetResource (model->GetName ());
	}

	ModelView* modelView = new ModelView ();

	/*
	 * Create buffer data
	*/

	std::map<std::size_t, std::size_t> indices;

	std::vector<NormalMapVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			std::size_t startIndex = indexBuffer.size ();

			for (Polygon* polygon : *polyGroup) {
				for(std::size_t j=0;j<polygon->VertexCount();j++) {

					std::size_t index = 0;

					std::size_t vertexPos = polygon->GetVertex (j);
					std::size_t normalPos = polygon->GetNormal (j);
					std::size_t texcoordPos = polygon->GetTexcoord (j);

					std::size_t hashPos = hash (vertexPos, hash (normalPos, texcoordPos));

					auto indexIt = indices.find (hashPos);

					if (indexIt == indices.end ()) {
						NormalMapVertexData vertexData;

						glm::vec3 position = model->GetVertex (polygon->GetVertex(j));
						vertexData.position[0] = position.x;
						vertexData.position[1] = position.y;
						vertexData.position[2] = position.z;

						if (polygon->HaveNormals ()) {
							glm::vec3 normal = model->GetNormal (polygon->GetNormal(j));
							vertexData.normal[0] = normal.x;
							vertexData.normal[1] = normal.y;
							vertexData.normal[2] = normal.z;
						}

						if (model->HaveUV()) {
							glm::vec2 texcoord = model->GetTexcoord (polygon->GetTexcoord(j));
							vertexData.texcoord[0] = texcoord.x;
							vertexData.texcoord[1] = texcoord.y;
						}

						if (model->HaveUV ()) {
							glm::vec3 tangent = CalculateTangent (model, polygon);

							vertexData.tangent [0] = tangent.x;
							vertexData.tangent [1] = tangent.y;
							vertexData.tangent [2] = tangent.z;
						}

						vertexBuffer.push_back (vertexData);

						index = vertexBuffer.size () - 1;

						indices [hashPos] = index;
					} else {
						index = indexIt->second;
					}

					indexBuffer.push_back (index);
				}
			}

			GroupBuffer groupBuffer;

			groupBuffer.materialView = LoadMaterial (polyGroup->GetMaterial ());
			groupBuffer.offset = startIndex;
			groupBuffer.INDEX_COUNT = indexBuffer.size () - startIndex;

			modelView->AddGroupBuffer (groupBuffer);
		}
	}

	ObjectBuffer objectBuffer = BindNormalMapModelVertexData (vertexBuffer, indexBuffer);

	modelView->SetObjectBuffer (objectBuffer);

	return Resource<ModelView> (modelView, model->GetName ());
}

Resource<ModelView> RenderSystem::LoadLightMapModel (const Resource<Model>& model)
{
	if (Resource<ModelView>::GetResource (model->GetName ()) != nullptr) {
		return Resource<ModelView>::GetResource (model->GetName ());
	}

	ModelView* modelView = new ModelView ();

	/*
	 * Create buffer data
	*/

	const LightMapModel* lmModel = dynamic_cast<const LightMapModel*> (&*model);

	std::map<std::size_t, std::size_t> indices;

	std::vector<LightMapVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			std::size_t startIndex = indexBuffer.size ();

			for (Polygon* polygon : *polyGroup) {
				for(std::size_t j=0;j<polygon->VertexCount();j++) {

					std::size_t index = 0;

					std::size_t vertexPos = polygon->GetVertex (j);
					std::size_t normalPos = polygon->GetNormal (j);
					std::size_t texcoordPos = polygon->GetTexcoord (j);

					std::size_t hashPos = hash (vertexPos, hash (normalPos, texcoordPos));

					auto indexIt = indices.find (hashPos);

					if (indexIt == indices.end ()) {
						LightMapVertexData vertexData;

						glm::vec3 position = model->GetVertex (polygon->GetVertex(j));
						vertexData.position[0] = position.x;
						vertexData.position[1] = position.y;
						vertexData.position[2] = position.z;

						if (polygon->HaveNormals ()) {
							glm::vec3 normal = model->GetNormal (polygon->GetNormal(j));
							vertexData.normal[0] = normal.x;
							vertexData.normal[1] = normal.y;
							vertexData.normal[2] = normal.z;
						}

						if (model->HaveUV()) {
							glm::vec2 texcoord = model->GetTexcoord (polygon->GetTexcoord(j));
							vertexData.texcoord[0] = texcoord.x;
							vertexData.texcoord[1] = texcoord.y;
						}

						if (lmModel->HaveLightMapUV ()) {
							glm::vec2 lmTexcoord = lmModel->GetLightMapTexcoord (polygon->GetTexcoord (j));
							vertexData.lmTexcoord [0] = lmTexcoord.x;
							vertexData.lmTexcoord [1] = lmTexcoord.y;
						}

						vertexBuffer.push_back (vertexData);

						index = vertexBuffer.size () - 1;

						indices [hashPos] = index;
					} else {
						index = indexIt->second;
					}

					indexBuffer.push_back (index);
				}
			}

			GroupBuffer groupBuffer;

			groupBuffer.materialView = LoadMaterial (polyGroup->GetMaterial ());
			groupBuffer.offset = startIndex;
			groupBuffer.INDEX_COUNT = indexBuffer.size () - startIndex;

			modelView->AddGroupBuffer (groupBuffer);
		}
	}

	ObjectBuffer objectBuffer = BindLightMapModelVertexData (vertexBuffer, indexBuffer);

	modelView->SetObjectBuffer (objectBuffer);

	return Resource<ModelView> (modelView, model->GetName ());
}

Resource<ModelView> RenderSystem::LoadTextGUI (const std::string& text, const Resource<Font>& font)
{
	//TODO: Fix this
	if (Resource<ModelView>::GetResource ("Text" + text) != nullptr) {
		return Resource<ModelView>::GetResource ("Text" + text);
	}

	ModelView* modelView = new ModelView ();

	/*
	 * Create buffer data
	*/

	ObjectBuffer objectBuffer = ProcessTextGUI (text, font);

	modelView->SetObjectBuffer (objectBuffer);

	GroupBuffer groupBuffer;
	groupBuffer.INDEX_COUNT = objectBuffer.PolygonsCount * 3;
	groupBuffer.offset = 0;

	modelView->AddGroupBuffer (groupBuffer);

	return Resource<ModelView> (modelView, "Text" + text);
}

void RenderSystem::CreateInstanceModelView (Resource<ModelView>& modelView,
	const std::vector<BufferAttribute>& attributes, std::size_t size, unsigned char* buffer)
{
	unsigned int instanceID;

	GL::GenBuffers(1, &instanceID);
	GL::BindBuffer(GL_ARRAY_BUFFER, instanceID);
	GL::BufferData(GL_ARRAY_BUFFER, size, buffer, GL_DYNAMIC_DRAW);

	ObjectBuffer& objectBuffer = modelView->GetObjectBuffer ();

	objectBuffer.VBO_INSTANCE_INDEX = instanceID;

	GL::BindVertexArray(objectBuffer.VAO_INDEX);
	GL::BindBuffer(GL_ARRAY_BUFFER, objectBuffer.VBO_INSTANCE_INDEX);

	for (BufferAttribute attr : attributes) {
		GL::EnableVertexAttribArray (attr.index);

		if (attr.type == BufferAttribute::AttrType::ATTR_F) {
			GL::VertexAttribPointer (attr.index, attr.size, GL_FLOAT, GL_FALSE, attr.stride, (void*) attr.pointer);
		}
		GL::VertexAttribDivisor (attr.index, 1);
	}
}

void RenderSystem::UpdateInstanceModelView (Resource<ModelView>& modelView, std::size_t size, std::size_t instancesCount, unsigned char* buffer)
{
	ObjectBuffer& objectBuffer = modelView->GetObjectBuffer ();

	GL::BindBuffer(GL_ARRAY_BUFFER, objectBuffer.VBO_INSTANCE_INDEX);
	GL::BufferSubData(GL_ARRAY_BUFFER, 0, size, buffer);

	objectBuffer.INSTANCES_COUNT = instancesCount;
}

Resource<MaterialView> RenderSystem::LoadMaterial (const Resource<Material>& material)
{
	if (material == nullptr) {
		return nullptr;
	}

	if (Resource<MaterialView>::GetResource (material->name) != nullptr) {
		return Resource<MaterialView>::GetResource (material->name);
	}

	MaterialView* materialView = new MaterialView ();

	ProcessMaterial (material, materialView);

	return Resource<MaterialView> (materialView, material->name);
}

Resource<TextureView> RenderSystem::LoadTexture (const Resource<Texture>& texture)
{
	if (Resource<TextureView>::GetResource (texture->GetName ()) != nullptr) {
		return Resource<TextureView>::GetResource (texture->GetName ());
	}

	TextureView* textureView = new TextureView ();

	/*
	 * Load texture in GPU
	*/

	textureView->SetGPUIndex (LoadTextureGPU (texture));

	return Resource<TextureView> (textureView, texture->GetName ());
}

Resource<TextureView> RenderSystem::LoadCubeMap (const Resource<Texture>& texture)
{
	if (Resource<TextureView>::GetResource (texture->GetName ()) != nullptr) {
		return Resource<TextureView>::GetResource (texture->GetName ());
	}

	CubeMapView* cubeMapView = new CubeMapView ();

	/*
	 * Load texture in GPU
	*/

	cubeMapView->SetGPUIndex (LoadCubeMapGPU (texture));

	return Resource<TextureView> (cubeMapView, texture->GetName ());
}

Resource<TextureView> RenderSystem::LoadTextureLUT (const Resource<Texture>& texture)
{
	if (Resource<TextureView>::GetResource (texture->GetName ()) != nullptr) {
		return Resource<TextureView>::GetResource (texture->GetName ());
	}

	TextureLUTView* lutTextureView = new TextureLUTView ();

	/*
	 * Load texture in GPU
	*/

	lutTextureView->SetGPUIndex (LoadTextureLUTGPU (texture));

	return Resource<TextureView> (lutTextureView, texture->GetName ());
}

Resource<ShaderView> RenderSystem::LoadShader (const Resource<Shader>& shader)
{
	if (Resource<ShaderView>::GetResource (shader->GetName ()) != nullptr) {
		return Resource<ShaderView>::GetResource (shader->GetName ());
	}

	const DrawingShader* drawingShader = dynamic_cast<const DrawingShader*> (&*shader);

	GLuint program = GL::CreateProgram ();

	/*
	 * Compile and attach vertex shader
	*/

	GLuint vertexShaderID = BuildShaderContent (drawingShader->GetVertexShaderContent (), GL_VERTEX_SHADER);

	GL::AttachShader (program, vertexShaderID);

	/*
	 * Compile and attach fragment shader
	*/

	GLuint fragmentShaderID = BuildShaderContent (drawingShader->GetFragmentShaderContent (), GL_FRAGMENT_SHADER);

	GL::AttachShader (program, fragmentShaderID);

	/*
	 * Compile and attach geometry shader if exists
	*/

	if (drawingShader->GetGeometryShaderContent () != nullptr) {
		unsigned int geometryShaderID = BuildShaderContent (drawingShader->GetGeometryShaderContent (), GL_GEOMETRY_SHADER);

		GL::AttachShader (program, geometryShaderID);
	}

	GL::LinkProgram (program);

	ShaderView* shaderView = new ShaderView (program);

	return Resource<ShaderView> (shaderView, shader->GetName ());
}

Resource<ShaderView> RenderSystem::LoadComputeShader (const Resource<Shader>& shader)
{
	const ComputeShader* computeShader = dynamic_cast<const ComputeShader*> (&*shader);

	unsigned int program = GL::CreateProgram();

	/*
	 * Load, compile and attach vertex shader
	*/

	GLuint computeShaderID = BuildShaderContent (computeShader->GetComputeShaderContent (), GL_COMPUTE_SHADER);

	GL::AttachShader (program, computeShaderID);

	GL::LinkProgram(program);

	ShaderView* shaderView = new ShaderView (program);

	return Resource<ShaderView> (shaderView, shader->GetName ());
}

Resource<Texture> RenderSystem::SaveTexture (const Resource<TextureView>& textureView)
{
	//TODO: Extend this
	Texture* texture = new Texture ("temp");

	/*
	 * Save texture in CPU
	*/

	GL::BindTexture (GL_TEXTURE_2D, textureView->GetGPUIndex ());
	
	int width, height;

	GL::GetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	GL::GetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	unsigned char* pixels = new unsigned char [width * height * 4];

	GL::GetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	texture->SetSize (Size (width, height));
	texture->SetPixels (pixels, width * height * 4);

	delete[] pixels;

	return Resource<Texture> (texture, texture->GetName ());
}

ObjectBuffer RenderSystem::BindModelVertexData (const std::vector<VertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays(1 , &VAO);
	GL::BindVertexArray(VAO);

	//creeaza vbo
	GL::GenBuffers(1, &VBO);
	GL::BindBuffer(GL_ARRAY_BUFFER, VBO);
	GL::BufferData(GL_ARRAY_BUFFER, sizeof(VertexData)*vBuf.size(), vBuf.data(), GL_STATIC_DRAW);
	
	//creeaza ibo
	GL::GenBuffers(1, &IBO);
	GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*iBuf.size(), iBuf.data(), GL_STATIC_DRAW);
	
	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray(0);																	//activare pipe 0
	GL::VertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray(1);																	//activare pipe 1
	GL::VertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float) * 3));		//trimite normale pe pipe 1
	GL::EnableVertexAttribArray(2);																	//activare pipe 2
	GL::VertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float) * 6));		//trimite texcoorduri pe pipe 2

	ObjectBuffer objectBuffer;
	objectBuffer.VAO_INDEX = VAO;
	objectBuffer.VBO_INDEX = VBO;
	objectBuffer.IBO_INDEX = IBO;
	objectBuffer.VBO_INSTANCE_INDEX = 0;

	objectBuffer.VerticesCount = vBuf.size ();
	objectBuffer.PolygonsCount = iBuf.size () / 3;

	return objectBuffer;
}

ObjectBuffer RenderSystem::BindAnimationModelVertexData (const std::vector<AnimatedVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays(1 , &VAO);
	GL::BindVertexArray(VAO);

	//creeaza vbo
	GL::GenBuffers(1, &VBO);
	GL::BindBuffer(GL_ARRAY_BUFFER, VBO);
	GL::BufferData(GL_ARRAY_BUFFER, sizeof(AnimatedVertexData)*vBuf.size(), &vBuf[0], GL_STATIC_DRAW);
	
	//creeaza ibo
	GL::GenBuffers(1, &IBO);
	GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*iBuf.size(), &iBuf[0], GL_STATIC_DRAW);
	
	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray(0);																			//activare pipe 0
	GL::VertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(AnimatedVertexData),(void*)0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray(1);																			//activare pipe 1
	GL::VertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(AnimatedVertexData),(void*)(sizeof(float) * 3));	//trimite normale pe pipe 1
	GL::EnableVertexAttribArray(2);																			//activare pipe 2
	GL::VertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(AnimatedVertexData),(void*)(sizeof(float) * 6));	//trimite texcoorduri pe pipe 2
	GL::EnableVertexAttribArray(3);																			//activare pipe 3
	GL::VertexAttribIPointer(3,4,GL_INT,sizeof(AnimatedVertexData),(void*)(sizeof (float) * 8)); 			//trimite indecsii de bone pe pipe 3
	GL::EnableVertexAttribArray(4);
	GL::VertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,sizeof(AnimatedVertexData),(void*)(sizeof(float) * 12));

	ObjectBuffer objectBuffer;
	objectBuffer.VAO_INDEX = VAO;
	objectBuffer.VBO_INDEX = VBO;
	objectBuffer.IBO_INDEX = IBO;
	objectBuffer.VBO_INSTANCE_INDEX = 0;

	objectBuffer.VerticesCount = vBuf.size ();
	objectBuffer.PolygonsCount = iBuf.size () / 3;

	return objectBuffer;
}

ObjectBuffer RenderSystem::BindNormalMapModelVertexData (const std::vector<NormalMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays (1, &VAO);
	GL::BindVertexArray (VAO);

	//creeaza vbo
	GL::GenBuffers (1, &VBO);
	GL::BindBuffer (GL_ARRAY_BUFFER, VBO);
	GL::BufferData (GL_ARRAY_BUFFER, sizeof (NormalMapVertexData)*vBuf.size (), &vBuf [0], GL_STATIC_DRAW);

	//creeaza ibo
	GL::GenBuffers (1, &IBO);
	GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (unsigned int)*iBuf.size (), &iBuf [0], GL_STATIC_DRAW);

	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray (0);																			//activare pipe 0
	GL::VertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) 0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray (1);																			//activare pipe 1
	GL::VertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) (sizeof (float) * 3));	//trimite normale pe pipe 1
	GL::EnableVertexAttribArray (2);																			//activare pipe 2
	GL::VertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) (sizeof (float) * 6));	//trimite texcoorduri pe pipe 2
	GL::EnableVertexAttribArray (3);																			//activare pipe 2
	GL::VertexAttribPointer (3, 3, GL_FLOAT, GL_FALSE, sizeof (NormalMapVertexData), (void*) (sizeof (float) * 8));	//trimite texcoorduri pe pipe 2

	ObjectBuffer objectBuffer;
	objectBuffer.VAO_INDEX = VAO;
	objectBuffer.VBO_INDEX = VBO;
	objectBuffer.IBO_INDEX = IBO;
	objectBuffer.VBO_INSTANCE_INDEX = 0;

	objectBuffer.VerticesCount = vBuf.size ();
	objectBuffer.PolygonsCount = iBuf.size () / 3;

	return objectBuffer;
}

/*
 * Thanks to: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
 *
 * Calculate vertex tangent based on explanation from the link above;
*/

glm::vec3 RenderSystem::CalculateTangent (const Resource<Model>& model, Polygon* poly)
{
	/*
	 * Tangents are generated only when texcoords are present
	*/

	if (!model->HaveUV ()) {
		return glm::vec3 (0.0f);
	}

	/*
	 * Get vertices
	*/

	glm::vec3 v0 = model->GetVertex(poly->GetVertex (0));
	glm::vec3 v1 = model->GetVertex(poly->GetVertex (1));
	glm::vec3 v2 = model->GetVertex(poly->GetVertex (2));

	/*
	 * Get texcoords
	*/

	glm::vec2 uv0 = model->GetTexcoord(poly->GetTexcoord (0));
	glm::vec2 uv1 = model->GetTexcoord (poly->GetTexcoord (1));
	glm::vec2 uv2 = model->GetTexcoord (poly->GetTexcoord (2));

	/*
	 * Edges of the triangle : postion delta
	*/

	glm::vec3 deltaPos1 = v1 - v0;
	glm::vec3 deltaPos2 = v2 - v0;

	/*
	 * UV delta
	*/

	glm::vec2 deltaUV1 = uv1 - uv0;
	glm::vec2 deltaUV2 = uv2 - uv0;

	/*
	 * "Here is the algorithm : if we note deltaPos1 and deltaPos2 two edges of
	 * our triangle, and deltaUV1 and deltaUV2 the corresponding differences in
	 * UVs, we can express our problem with the following equation :
	 *
	 * deltaPos1 = deltaUV1.x * T + deltaUV1.y * B
	 * deltaPos2 = deltaUV2.x * T + deltaUV2.y * B
	 *
	 * Just solve this system for T and B, and you have your vectors !
	*/

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

	/*
	 * Return the tangent
	*/

	return glm::normalize (tangent);
}

ObjectBuffer RenderSystem::BindLightMapModelVertexData (const std::vector<LightMapVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays (1, &VAO);
	GL::BindVertexArray (VAO);

	//creeaza vbo
	GL::GenBuffers (1, &VBO);
	GL::BindBuffer (GL_ARRAY_BUFFER, VBO);
	GL::BufferData (GL_ARRAY_BUFFER, sizeof (LightMapVertexData)*vBuf.size (), &vBuf [0], GL_STATIC_DRAW);

	//creeaza ibo
	GL::GenBuffers (1, &IBO);
	GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (unsigned int)*iBuf.size (), &iBuf [0], GL_STATIC_DRAW);

	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray (0);																			//activare pipe 0
	GL::VertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) 0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray (1);																			//activare pipe 1
	GL::VertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) (sizeof (float) * 3));	//trimite normale pe pipe 1
	GL::EnableVertexAttribArray (2);																			//activare pipe 2
	GL::VertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) (sizeof (float) * 6));	//trimite texcoorduri pe pipe 2
	GL::EnableVertexAttribArray (3);																			//activare pipe 2
	GL::VertexAttribPointer (3, 2, GL_FLOAT, GL_FALSE, sizeof (LightMapVertexData), (void*) (sizeof (float) * 8));	//trimite texcoorduri pe pipe 2

	ObjectBuffer objectBuffer;
	objectBuffer.VAO_INDEX = VAO;
	objectBuffer.VBO_INDEX = VBO;
	objectBuffer.IBO_INDEX = IBO;
	objectBuffer.VBO_INSTANCE_INDEX = 0;

	objectBuffer.VerticesCount = vBuf.size ();
	objectBuffer.PolygonsCount = iBuf.size () / 3;

	return objectBuffer;
}

ObjectBuffer RenderSystem::ProcessTextGUI (const std::string& text, const Resource<Font>& font)
{
	std::vector<TextGUIVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	glm::vec2 screenPos (0.0f);

	Resource<Texture> texture = font->GetTexture (0);

	for (std::size_t i=0;i<text.size ();i++) {
		TextGUIVertexData vertexData;

		FontChar* ch = font->GetChar (text [i]);

		glm::vec2 texSize (texture->GetSize ().width, texture->GetSize ().height);
		glm::vec2 scale = (glm::vec2) ch->GetSize ();
		glm::vec2 offset = glm::vec2(ch->GetOffset ().x, -ch->GetOffset ().y);

		glm::vec2 upLeft = screenPos + offset;
		glm::vec2 tUpLeft = ((glm::vec2) ch->GetPosition ()) / texSize;

		vertexData.position [0] = upLeft.x; vertexData.position [1] = upLeft.y;
		vertexData.texcoord [0] = tUpLeft.x; vertexData.texcoord [1] = tUpLeft.y;

		vertexBuffer.push_back (vertexData);

		glm::vec2 upRight = screenPos + glm::vec2 (scale.x, 0) + offset;
		glm::vec2 tUpRight = tUpLeft + (glm::vec2 (ch->GetSize ().x, 0)) / texSize;

		vertexData.position [0] = upRight.x; vertexData.position [1] = upRight.y;
		vertexData.texcoord [0] = tUpRight.x; vertexData.texcoord [1] = tUpRight.y;

		vertexBuffer.push_back (vertexData);

		glm::vec2 downLeft = screenPos + glm::vec2 (0, -scale.y) + offset;
		glm::vec2 tDownLeft = tUpLeft + (glm::vec2 (0, ch->GetSize ().y)) / texSize;

		vertexData.position [0] = downLeft.x; vertexData.position [1] = downLeft.y;
		vertexData.texcoord [0] = tDownLeft.x; vertexData.texcoord [1] = tDownLeft.y;

		vertexBuffer.push_back (vertexData);

		glm::vec2 downRight = screenPos + glm::vec2 (scale.x, -scale.y) + offset;
		glm::vec2 tDownRight = tDownLeft + (glm::vec2 (ch->GetSize ().x, 0)) / texSize;

		vertexData.position [0] = downRight.x; vertexData.position [1] = downRight.y;
		vertexData.texcoord [0] = tDownRight.x; vertexData.texcoord [1] = tDownRight.y;

		vertexBuffer.push_back (vertexData);

		unsigned int iUpLeft = (unsigned int) i * 4, iUpRight = (unsigned int) i * 4 + 1,
			iDownLeft = (unsigned int) i * 4 + 2, iDownRight = (unsigned int) i * 4 + 3;

		indexBuffer.push_back (iUpLeft); indexBuffer.push_back (iDownLeft); indexBuffer.push_back (iUpRight);
		indexBuffer.push_back (iUpRight); indexBuffer.push_back (iDownLeft); indexBuffer.push_back (iDownRight);

		screenPos.x += ch->GetXAdvance ();
	}	

	ObjectBuffer objectBuffer = BindTextGUIVertexData (vertexBuffer, indexBuffer);

	return objectBuffer;
}

ObjectBuffer RenderSystem::BindTextGUIVertexData (const std::vector<TextGUIVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
{
	unsigned int VAO, VBO, IBO;

	//creaza vao
	GL::GenVertexArrays(1 , &VAO);
	GL::BindVertexArray(VAO);

	//creeaza vbo
	GL::GenBuffers(1, &VBO);
	GL::BindBuffer(GL_ARRAY_BUFFER, VBO);
	GL::BufferData(GL_ARRAY_BUFFER, sizeof(TextGUIVertexData)*vBuf.size(), vBuf.data(), GL_STATIC_DRAW);
	
	//creeaza ibo
	GL::GenBuffers(1, &IBO);
	GL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	GL::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*iBuf.size(), iBuf.data(), GL_STATIC_DRAW);
	
	// metoda 1: seteaza atribute folosind pipe-urile interne ce fac legatura OpenGL - GLSL, in shader folosim layout(location = pipe_index)
	// metoda cea mai buna, specificare explicita prin qualificator layout)
	GL::EnableVertexAttribArray(0);																	//activare pipe 0
	GL::VertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(TextGUIVertexData),(void*)0);						//trimite pozitii pe pipe 0
	GL::EnableVertexAttribArray(1);																	//activare pipe 1
	GL::VertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(TextGUIVertexData),(void*)(sizeof(float) * 2));		//trimite normale pe pipe 1

	ObjectBuffer objectBuffer;
	objectBuffer.VAO_INDEX = VAO;
	objectBuffer.VBO_INDEX = VBO;
	objectBuffer.IBO_INDEX = IBO;
	objectBuffer.VBO_INSTANCE_INDEX = 0;

	objectBuffer.VerticesCount = vBuf.size ();
	objectBuffer.PolygonsCount = iBuf.size () / 3;

	return objectBuffer;
}

void RenderSystem::ProcessMaterial (const Resource<Material>& material, MaterialView* materialView)
{
	materialView->ambientColor = material->ambientColor;
	materialView->diffuseColor = material->diffuseColor;
	materialView->specularColor = material->specularColor;
	materialView->shininess = material->shininess;
	materialView->transparency = material->transparency;
	materialView->refractiveIndex = material->refractiveIndex;

	if (material->ambientTexture != nullptr) {
		materialView->ambientTexture = LoadTexture (material->ambientTexture);
	}

	if (material->diffuseTexture != nullptr) {
		materialView->diffuseTexture = LoadTexture (material->diffuseTexture);
	}

	if (material->specularTexture != nullptr) {
		materialView->specularTexture = LoadTexture (material->specularTexture);
	}

	if (material->alphaTexture != nullptr) {
		materialView->alphaTexture = LoadTexture (material->alphaTexture);
	}

	if (material->bumpTexture != nullptr) {
		materialView->bumpTexture = LoadTexture (material->bumpTexture);
	}

	if (material->cubeTexture != nullptr) {
		materialView->cubeTexture = LoadTexture (material->cubeTexture);
	}

	if (material->shader != nullptr) {
		materialView->shaderView = LoadShader (material->shader);
	}
}

unsigned int RenderSystem::LoadTextureGPU (const Resource<Texture>& texture)
{
	unsigned int gpuIndex = 0;

	Size size (texture->GetSize ());

	GL::GenTextures(1, &gpuIndex);
	GL::BindTexture(GL_TEXTURE_2D, gpuIndex);

	/*
	 * Get Pixel Format
	*/

	int pixelFormat = GL_RGBA;

	switch (texture->GetPixelFormat ()) {
		case FORMAT_RGB8:
			pixelFormat = GL_RGB;
			break;
		case FORMAT_BGR8:
			pixelFormat = GL_BGR;
			break;
		case FORMAT_RGBA8:
			pixelFormat = GL_RGBA;
			break;
		case FORMAT_ABGR8:
			pixelFormat = GL_BGRA;
			break;
		default:
			pixelFormat = GL_RGBA;
			break;
	}

	/*
	 * Send MipMaps to GPU
	*/

	for (std::size_t i=0;i<texture->GetMipMapLevels ();i++) {
		int internalFormat = texture->GetInternalFormat ();

		if (internalFormat == 0) {
			internalFormat = GL_RGBA;
		}

		GL::TexImage2D(GL_TEXTURE_2D, i, internalFormat, size.width >> i, size.height >> i, 0, pixelFormat, GL_UNSIGNED_BYTE, texture->GetMipmapLevel (i));
	}

	/*
	 * Wrap Mode
	*/

	if (texture->GetWrapMode () == TEXTURE_WRAP_MODE::WRAP_REPEAT) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);		
	}
	else if (texture->GetWrapMode () == TEXTURE_WRAP_MODE::WRAP_CLAMP) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);		
	}

	/*
	 * Magnifying and minifying properties
	*/

	if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_NEAREST) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_BILINEAR) {
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		GL::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	else if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_ANISOTROPIC) {
		GLfloat maxAnisotropy;
		GL::GetFloatv (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

		maxAnisotropy = Extensions::MathExtend::Clamp (maxAnisotropy, 0.0f, 8.0f);
		GL::TexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	} 
	else if (texture->GetMipmapFilter () == TEXTURE_MIPMAP_FILTER::MIPMAP_NONE) {
		GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	/*
	 * Generate mipmaps
	 *
	 * TODO: Extend this to create mipmaps with multiple filters.
	*/

	if (texture->GenerateMipmaps () && !texture->HasMipmaps ()) {
		GL::GenerateMipmap (GL_TEXTURE_2D);
	}

	/*
	 * Unbind current texture index
	*/

	GL::BindTexture (GL_TEXTURE_2D, 0);

	/*
	 * Update texture
	*/

	return gpuIndex;
}

unsigned int RenderSystem::LoadCubeMapGPU (const Resource<Texture>& texture)
{
	const CubeMap* cubemap = dynamic_cast<const CubeMap*> (&*texture);

	unsigned int gpuIndex;
	GL::GenTextures (1, &gpuIndex);
	GL::BindTexture (GL_TEXTURE_CUBE_MAP, gpuIndex);

	for (std::size_t i=0;i<6;i++) {
		Resource<Texture> face = cubemap->GetTexture (i);
		Size size (face->GetSize ());

		unsigned int num;
		GL::GenTextures(1,&num);

		GL::BindTexture(GL_TEXTURE_2D,num);

		GL::TexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, face->GetPixels ());

		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);        
		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);      //we do it for vertically and horizontally (previous line)		
	}

	return gpuIndex;
}

unsigned int RenderSystem::LoadTextureLUTGPU (const Resource<Texture>& texture)
{
	unsigned int gpuIndex = 0;

	Size size (texture->GetSize ());

	GL::GenTextures(1, &gpuIndex);
	GL::BindTexture(GL_TEXTURE_3D, gpuIndex);

	/*
	 * Texture properties
	*/

	GL::TexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	GL::TexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	GL::TexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	GL::TexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	GL::TexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

	GL::TexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, size.height, size.height, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetPixels ());

	/*
	 * Unbind current texture index
	*/

	GL::BindTexture (GL_TEXTURE_3D, 0);

	/*
	 * Update texture
	*/

	return gpuIndex;
}

/*
 * Compile shader based on type (vertex, geometry, fragment, compute)
*/

unsigned int RenderSystem::BuildShaderContent (const Resource<ShaderContent>& shaderContent, int shaderType)
{
	unsigned int shaderID = GL::CreateShader ((GLenum)shaderType);

	const char* csource = shaderContent->GetContent ().c_str ();

	Console::Log ("Compiling \"" + shaderContent->GetFilename () + "\" !");

	GL::ShaderSource (shaderID, 1, &csource, NULL);
	GL::CompileShader (shaderID);

	ShaderErrorCheck (shaderID);

	return shaderID;
}

bool RenderSystem::ShaderErrorCheck (unsigned int shader)
{
	int isCompiled;
	GL::GetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if(isCompiled == GL_FALSE) {
		int maxLength = 0;
		GL::GetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		GL::GetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		std::string error(errorLog.begin(), errorLog.end());
		Console::LogError (error);

		GL::DeleteShader(shader); // Don't leak the shader.

		return false;
	}

	return true;
}
