#include "GenericObjectModelLoader.h"

#include "Mesh/ObjectModel.h"
#include "Mesh/PolygonGroup.h"
#include "Mesh/Polygon.h"

#include "Material/Material.h"
#include "Texture/Texture.h"

#include "Managers/MaterialManager.h"
#include "Managers/TextureManager.h"

#include "Resources/Resources.h"

#include "Utils/Extensions/StringExtend.h"
#include "Utils/Files/FileSystem.h"

#include "Core/Console/Console.h"

Object* GenericObjectModelLoader::Load (const std::string& fileName)
{
	Model* model = new Model ();
	model->SetName ("test");

	Assimp::Importer assimpImporter;

	const aiScene* assimpScene = assimpImporter.ReadFile (fileName, 
		aiProcess_GenSmoothNormals | 
		aiProcess_Triangulate | 
		aiProcess_CalcTangentSpace | 
		aiProcess_FlipUVs);

	if (assimpScene == nullptr) {
		Console::LogError ("Unable to load \"" + fileName + "\" model!");

		exit (0);
	}

	if (assimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || assimpScene->mRootNode == nullptr) {
		Console::LogError ("Unable to scuccessfully process \"" + fileName + "\" model!");

		exit (0);
	}

	ProcessTree (model, assimpScene->mRootNode, assimpScene, fileName);

	return model;
}

void GenericObjectModelLoader::ProcessTree (Model* model, aiNode* assimpNode, const aiScene* assimpScene, const std::string& filename)
{
	for (std::size_t i=0;i<assimpNode->mNumMeshes;i++) {
		aiMesh* assimpMesh = assimpScene->mMeshes [assimpNode->mMeshes [i]];
		ProcessObject (model, assimpMesh, assimpScene, filename);
	}

	for (std::size_t i=0;i<assimpNode->mNumChildren;i++) {
		ProcessTree (model, assimpNode->mChildren [i], assimpScene, filename);
	}
}

void GenericObjectModelLoader::ProcessObject (Model* model, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename)
{
	ObjectModel* objectModel = new ObjectModel (std::string (assimpMesh->mName.C_Str ()));

	ProcessFaces (model, objectModel, assimpMesh, assimpScene, filename);

	ProcessVertices (model, assimpMesh);
	ProcessNormals (model, assimpMesh);
	ProcessTexcoords (model, assimpMesh);

	model->AddObjectModel (objectModel);
}

void GenericObjectModelLoader::ProcessVertices (Model* model, aiMesh* assimpMesh)
{
	for (std::size_t i=0;i<assimpMesh->mNumVertices;i++)
	{
		glm::vec3* vertex = new glm::vec3 ();

		vertex->x = (float) assimpMesh->mVertices [i].x;
		vertex->y = (float) assimpMesh->mVertices [i].y;
		vertex->z = (float) assimpMesh->mVertices [i].z;

		model->AddVertex (vertex);
	}
}

void GenericObjectModelLoader::ProcessNormals (Model* model, aiMesh* assimpMesh)
{
	for (std::size_t i=0;i<assimpMesh->mNumVertices;i++)
	{
		glm::vec3* normal = new glm::vec3 ();

		normal->x = (float) assimpMesh->mNormals [i].x;
		normal->y = (float) assimpMesh->mNormals [i].y;
		normal->z = (float) assimpMesh->mNormals [i].z;

		model->AddNormal (normal);
	}
}

void GenericObjectModelLoader::ProcessTexcoords (Model* model, aiMesh* assimpMesh)
{
	if (assimpMesh->mTextureCoords[0] == nullptr) {
		return ;
	}

	for (std::size_t i=0;i<assimpMesh->mNumVertices;i++)
	{
		glm::vec3* texcoord = new glm::vec3 ();

		texcoord->x = (float) assimpMesh->mTextureCoords [0][i].x;
		texcoord->y = (float) assimpMesh->mTextureCoords [0][i].y;

		model->AddTexcoord (texcoord);
	}	
}

void GenericObjectModelLoader::ProcessFaces (Model* model, ObjectModel* objectModel, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename)
{
	PolygonGroup* polyGroup = new PolygonGroup (objectModel->GetName ());

	for (std::size_t i=0;i<assimpMesh->mNumFaces;i++) {
		aiFace assimpFace =  assimpMesh->mFaces [i];

		Polygon* polygon = new Polygon ();

		for (std::size_t j=0;j<assimpFace.mNumIndices;j++) {
			polygon->AddVertex ((int) model->VertexCount () + assimpFace.mIndices [j]);
			polygon->AddNormal ((int) model->VertexCount () + assimpFace.mIndices [j]);

			if (assimpMesh->mTextureCoords[0]) {
				polygon->AddTexcoord ((int) model->VertexCount () + assimpFace.mIndices [j]);
			}
		}

		polyGroup->AddPolygon (polygon);
	}

	ProcessMaterial (polyGroup, assimpMesh, assimpScene, filename);

	objectModel->AddPolygonGroup (polyGroup);
}

void GenericObjectModelLoader::ProcessMaterial (PolygonGroup* polyGroup, aiMesh* assimpMesh, const aiScene* assimpScene, const std::string& filename)
{
	Material* material = new Material ();

	/*
	 * TODO: Change this to random hash
	*/

	material->name = polyGroup->GetName () + std::to_string (polyGroup->GetPolygonCount ()) + "_mat";

	aiMaterial* assimpMaterial = assimpScene->mMaterials [assimpMesh->mMaterialIndex];

	for (unsigned int i=0;i<assimpMaterial->GetTextureCount (aiTextureType_DIFFUSE); i++) {
		aiString textureNameS;
		assimpMaterial->GetTexture (aiTextureType_DIFFUSE, i, &textureNameS);
		std::string textureName = textureNameS.C_Str ();
		Extensions::StringExtend::Trim (textureName, '/');

		std::string directory = FileSystem::GetDirectory (filename);
		textureName = directory + textureName;

		Texture* texture = TextureManager::Instance ()->GetTexture (textureName);
		if (texture == nullptr) {
			texture = Resources::LoadTexture (std::string (textureName));
			TextureManager::Instance ()->AddTexture (texture);
		}

		material->diffuseTexture = texture->GetGPUIndex ();
	}

	aiColor4D col;
	aiGetMaterialColor (assimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &col);
	material->diffuseColor = glm::vec3 (col.r, col.g, col.b);

	polyGroup->SetMaterialName (material->name);
	MaterialManager::Instance ().AddMaterial (material);
}

/*
#include "sceneLoader.h"
	
void meshLoader::recursiveProcess(aiNode* node,const aiScene* scene)
{
	//process
	for(int i=0;i<node->mNumMeshes;i++)
	{
		aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh,scene);
	}
	
	
	
	//recursion
	for(int i=0;i<node->mNumChildren;i++)
	{
		recursiveProcess(node->mChildren[i],scene);
	}
}

void meshLoader::processMesh(aiMesh* mesh,const aiScene* scene)
{
	std::vector<vertexData> data;
	std::vector<unsigned int> indices;
	std::vector<textureData> textures; 
	aiColor4D col;
	aiMaterial* mat=scene->mMaterials[mesh->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	vector3d defaultColor(col.r,col.g,col.b);
	
		
	for(int i=0;i<mesh->mNumVertices;i++)
	{
		vertexData tmp;
		vector3d tmpVec;
		
		//position
		tmpVec.x=mesh->mVertices[i].x;
		tmpVec.y=mesh->mVertices[i].y;
		tmpVec.z=mesh->mVertices[i].z;
		tmp.position=tmpVec;
		
		//normals
		tmpVec.x=mesh->mNormals[i].x;
		tmpVec.y=mesh->mNormals[i].y;
		tmpVec.z=mesh->mNormals[i].z;
		tmp.normal=tmpVec;			
		
		
		//tangent
		if(mesh->mTangents)
		{
			tmpVec.x=mesh->mTangents[i].x;
			tmpVec.y=mesh->mTangents[i].y;
			tmpVec.z=mesh->mTangents[i].z;
		}else{
			tmpVec.x=1.0;
			tmpVec.y=tmpVec.z=0;
		}
		tmp.tangent=tmpVec;		
		
		
		//colors
		if(mesh->mColors[0])
		{
			//!= material color
			tmpVec.x=mesh->mColors[0][i].r;
			tmpVec.y=mesh->mColors[0][i].g;
			tmpVec.z=mesh->mColors[0][i].b;				
		}else{
			tmpVec=defaultColor;
		}
		tmp.color=tmpVec;
		
		//color
		if(mesh->mTextureCoords[0])
		{
			tmpVec.x=mesh->mTextureCoords[0][i].x;
			tmpVec.y=mesh->mTextureCoords[0][i].y;				
		}else{
			tmpVec.x=tmpVec.y=tmpVec.z=0.0;
		}
		tmp.U=tmpVec.x;
		tmp.V=tmpVec.y;
		data.push_back(tmp);
	}
	
	for(int i=0;i<mesh->mNumFaces;i++)
	{
		aiFace face=mesh->mFaces[i];
		for(int j=0;j<face.mNumIndices;j++) //0..2
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	
	for(int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
		textureData tmp;
		tmp.id=loadTexture(str.C_Str());
		tmp.type=0;
		textures.push_back(tmp);
	}
	meshes.push_back(new mesh::mesh(&data,&indices,&textures));	
}

unsigned int meshLoader::loadTexture(const char* filename)
{
	unsigned int num;
	glGenTextures(1,&num);
	SDL_Surface* img=IMG_Load(filename);
	if(img==NULL)
	{
		//std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);
	if(img2==NULL)
	{
		//std::cout << "img2 was not loaded" << std::endl;
		return -1;		
	}
	glBindTexture(GL_TEXTURE_2D,num);		
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);		
	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);
	return num;	
}


meshLoader::meshLoader(const char* filename)
{
	Assimp::Importer importer;
	const aiScene* scene=importer.ReadFile(filename, aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
	if(scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "The file wasn't successfuly opened " << filename << std::endl;
		return; 
	}
	
	recursiveProcess(scene->mRootNode,scene);
}

meshLoader::~meshLoader()
{
	for(int i=0;i<meshes.size();i++)
		delete meshes[i];
}

void meshLoader::draw(unsigned int programId)
{
	for(int i=0;i<meshes.size();i++)
		meshes[i]->draw(programId);	
}

std::vector<mesh*>& meshLoader::getMeshes()
{
	return meshes;
}
*/