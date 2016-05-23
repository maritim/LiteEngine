#include "AnimationModel3DRenderer.h"

#include <assimp/scene.h>
#include <vector>

#include "Core/Math/Vector3.h"

#include "Renderer/Pipeline.h"

#include "Material/Material.h"
#include "Mesh/Polygon.h"
#include "Mesh/VertexBoneInfo.h"
#include "Mesh/AnimationsController.h"
#include "Mesh/BoneTree.h"

#include "Managers/ShaderManager.h"
#include "Managers/MaterialManager.h"

#include "Wrappers/OpenGL/GL.h"
#include "Systems/Time/Time.h"

#include "Utils/Conversions/Matrices.h"

#include "Core/Debug/Debug.h"

void AnimationModel3DRenderer::Attach (Model* model)
{
	for (std::size_t i=0;i<model->ObjectsCount ();i++) {
		ProcessObjectModel (model, model->GetObject (i));
	}

	_animationModel = dynamic_cast<AnimationModel*> (model);
}

void AnimationModel3DRenderer::Draw ()
{
	GL::DepthMask (GL_TRUE);

	Pipeline::SetObjectTransform (_transform);
	
	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Material* mat = MaterialManager::Instance ().GetMaterial (_drawableObjects [i].MAT_NAME);

		if (mat == nullptr) {
			mat = MaterialManager::Instance ().Default ();
		}

		GL::BlendFunc (mat->blending.first, mat->blending.second);

		Pipeline::SendMaterial (mat, ShaderManager::Instance ()->GetShader ("DEFAULT_ANIMATED"));

		std::vector<PipelineAttribute> customAttributes = GetCustomAttributes ();
		Pipeline::SendCustomAttributes ("DEFAULT_ANIMATED", customAttributes);

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray(_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElements (GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0);
	}
}

// For the moment I clone the vertex/normal/texture tuple for every one
// Maybe will be a good idea to delete the duplicates, low priority TODO:
BufferObject AnimationModel3DRenderer::ProcessPolygonGroup (Model* model, PolygonGroup* polyGroup)
{
	AnimationModel* animModel = dynamic_cast<AnimationModel*> (model);

	std::vector<AnimatedVertexData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;

	for (std::size_t i=0;i<polyGroup->GetPolygonCount ();i++) {
		Polygon* polygon = polyGroup->GetPolygon (i);

		for(std::size_t j=0;j<polygon->VertexCount();j++) {
			AnimatedVertexData vertexData;

			Vector3* position = animModel->GetVertex (polygon->GetVertex(j));
			vertexData.position[0] = position->x;
			vertexData.position[1] = position->y;
			vertexData.position[2] = position->z;

			if (polygon->HaveNormals ()) {
				Vector3* normal = animModel->GetNormal (polygon->GetNormal(j));
				vertexData.normal[0] = normal->x;
				vertexData.normal[1] = normal->y;
				vertexData.normal[2] = normal->z;
			}

			if (animModel->HaveUV()) {
				Vector3* texcoord = animModel->GetTexcoord (polygon->GetTexcoord(j));
				vertexData.texcoord[0] = texcoord->x;
				vertexData.texcoord[1] = texcoord->y;
			}

			/*
			 * Add bone information per vertex
			*/

			VertexBoneInfo* vertexBoneInfo = animModel->GetVertexBoneInfo (polygon->GetVertex (j));

			for (std::size_t k=0;k<4 && k<vertexBoneInfo->GetBoneIDsCount ();k++) {
				vertexData.bones [k] = vertexBoneInfo->GetBoneID (k);
				vertexData.weights [k] = vertexBoneInfo->GetBoneWeight (k);
			}

			vertexBuffer.push_back (vertexData);
		}

		indexBuffer.push_back (3 * i);
		indexBuffer.push_back (3 * i + 1);
		indexBuffer.push_back (3 * i + 2);
	}

	BufferObject bufObj = BindVertexData (vertexBuffer, indexBuffer);
	bufObj.MAT_NAME = polyGroup->GetMaterialName ();

	return bufObj;
}

std::vector<PipelineAttribute> AnimationModel3DRenderer::GetCustomAttributes ()
{
	// Calculate attribute
	std::vector<aiMatrix4x4> boneTransform (_animationModel->GetBoneCount ());

	BoneTree* boneTree = _animationModel->GetBoneTree ();
	AnimationsController* animController = _animationModel->GetAnimationsController ();
	// AnimationContainer* animContainer = animController->GetAnimationContainer ("combinedAnim_0");
	AnimationContainer* animContainer = animController->GetAnimationContainer ("");
	aiMatrix4x4 rootMatrix;

	aiMatrix4x4 globalInverse = boneTree->GetRoot ()->GetTransform ();
	globalInverse = globalInverse.Inverse ();

	float animationTime = Time::GetTime () / 1000;
	float ticksPerSecond = animContainer->GetTicksPerSecond () != 0 ? animContainer->GetTicksPerSecond () : 25.0f;
	float timeInTicks = animationTime * ticksPerSecond;
	animationTime = std::fmod (timeInTicks, animContainer->GetDuration ());

 	DEBUG_LOG ("START");
	ProcessBoneTransform (_animationModel, rootMatrix, animContainer, boneTree->GetRoot (), globalInverse, animationTime, boneTransform);

	// Create attribute
	std::vector<PipelineAttribute> attributes;

	for (std::size_t i=0;i<boneTransform.size ();i++) {
	
		PipelineAttribute boneTransformAttr;

		boneTransformAttr.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

		boneTransformAttr.name = "boneTransforms[" + std::to_string (i) + "]";

		boneTransformAttr.matrix = Conversions::AssimpMatrixToGLMMat (boneTransform [i]);

		attributes.push_back (boneTransformAttr);
	}

	return attributes;
}

void AnimationModel3DRenderer::ProcessBoneTransform (AnimationModel* animModel, aiMatrix4x4 parentTransform, 
	AnimationContainer* animContainer, BoneNode* boneNode, aiMatrix4x4 inverseGlobalMatrix,
	float animationTime, std::vector<aiMatrix4x4>& boneTransform)
{
	std::string nodeName = boneNode->GetName ();
	aiMatrix4x4 nodeTransform = boneNode->GetTransform ();

	DEBUG_LOG (nodeName);

	AnimationNode* animNode = animContainer->GetAnimationNode (nodeName);

	if (animNode != nullptr) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D scaling;
		CalcInterpolatedScaling (scaling, animationTime, animNode);
		aiMatrix4x4 scalingM;// = GetScalingMatrix (scaling);
		aiMatrix4x4::Scaling (scaling, scalingM);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion rotationQ;
		CalcInterpolatedRotation(rotationQ, animationTime, animNode);
		aiMatrix4x4 rotationM = GetRotationMatrix (rotationQ);

		// Interpolate translation and generate translation transformation matrixa
		aiVector3D translation;
		CalcInterpolatedPosition(translation, animationTime, animNode);
		aiMatrix4x4 translationM;// = GetTranslationMatrix (translation);
		aiMatrix4x4::Translation (translation, translationM);

		nodeTransform = translationM * rotationM * scalingM;
	}

	aiMatrix4x4 globalTransformation = parentTransform * nodeTransform;

	BoneInfo* boneInfo = animModel->GetBone (nodeName);

	if (boneInfo != nullptr) {
		boneTransform [boneInfo->GetID ()] = inverseGlobalMatrix * globalTransformation * boneInfo->GetTransformMatrix ();
	}

	for (std::size_t i=0;i<boneNode->GetChildrenCount ();i++) {
		ProcessBoneTransform (animModel, globalTransformation, animContainer, boneNode->GetChild (i), 
			inverseGlobalMatrix, animationTime, boneTransform);
	}
}

void AnimationModel3DRenderer::CalcInterpolatedRotation(aiQuaternion& rotationQ, float animationTime, AnimationNode* animNode)
{
		rotationQ = animNode->GetRotationKey (0).mValue;
		return;
	if (animNode->GetRotationKeysCount () == 1) {
		rotationQ = animNode->GetRotationKey (0).mValue;
		return ;
	}	

	std::size_t rotationIndex = 0;
	std::size_t nextRotationIndex = 0;

	while (rotationIndex < animNode->GetScalingKeysCount () - 1 &&
		animNode->GetScalingKey (rotationIndex + 1).mTime < animationTime) {
		++ rotationIndex;
	}

	nextRotationIndex = rotationIndex + 1;

	float deltaTime = animNode->GetRotationKey (nextRotationIndex).mTime - animNode->GetRotationKey (rotationIndex).mTime;
	float factor = (animationTime - animNode->GetRotationKey (rotationIndex).mTime) / deltaTime;

	const aiQuaternion& startRotationQ = animNode->GetRotationKey (rotationIndex).mValue;
	const aiQuaternion& endRotationQ = animNode->GetRotationKey (nextRotationIndex).mValue;

	aiQuaternion::Interpolate (rotationQ, startRotationQ, endRotationQ, factor);
	rotationQ = rotationQ.Normalize ();
}

void AnimationModel3DRenderer::CalcInterpolatedScaling (aiVector3D& scalingV, float animationTime, AnimationNode* animNode)
{
	scalingV = animNode->GetScalingKey (0).mValue;

	return;
	if (animNode->GetScalingKeysCount () == 1) {
		scalingV = animNode->GetScalingKey (0).mValue;
		return ;
	}	

	std::size_t scalingIndex = 0;
	std::size_t nextScalingIndex = 0;

	while (scalingIndex < animNode->GetScalingKeysCount () - 1 &&
		animNode->GetScalingKey (scalingIndex + 1).mTime < animationTime) {
		++ scalingIndex;
	}

	nextScalingIndex = scalingIndex + 1;

	float deltaTime = animNode->GetScalingKey (nextScalingIndex).mTime - animNode->GetScalingKey (scalingIndex).mTime;
	float factor = (animationTime - animNode->GetScalingKey (scalingIndex).mTime) / deltaTime;

	const aiVector3D& startScalingV = animNode->GetScalingKey (scalingIndex).mValue;
	const aiVector3D& endScalingV = animNode->GetScalingKey (nextScalingIndex).mValue;

	aiVector3D delta = endScalingV - startScalingV;

	scalingV = startScalingV + factor * delta;

	scalingV.Normalize ();
} 

void AnimationModel3DRenderer::CalcInterpolatedPosition (aiVector3D& positionV, float animationTime, AnimationNode* animNode)
{
		positionV = animNode->GetPositionKey (0).mValue;
	return;
	if (animNode->GetPositionKeysCount () == 1) {
		positionV = animNode->GetPositionKey (0).mValue;
		return ;
	}

	std::size_t positionIndex = 0;
	std::size_t nextPositionIndex = 0;

	while (positionIndex < animNode->GetPositionKeysCount () - 1 &&
		animNode->GetPositionKey (positionIndex + 1).mTime < animationTime) {
		++ positionIndex;
	}

	nextPositionIndex = positionIndex + 1;

	float deltaTime = animNode->GetPositionKey (nextPositionIndex).mTime - animNode->GetPositionKey (positionIndex).mTime;
	float factor = (animationTime - (float)animNode->GetPositionKey (positionIndex).mTime) / deltaTime;

	const aiVector3D& startTranslationV = animNode->GetPositionKey (positionIndex).mValue;
	const aiVector3D& endTranslationV = animNode->GetPositionKey (nextPositionIndex).mValue;
 
	aiVector3D delta = endTranslationV - startTranslationV;

	positionV = startTranslationV + factor * delta;

	positionV.Normalize ();
} 

aiMatrix4x4 AnimationModel3DRenderer::GetRotationMatrix (aiQuaternion rotation)
{
	aiMatrix4x4 matrix;

	aiMatrix3x3 rotationSubMatrix = rotation.GetMatrix ();

	for (std::size_t i=0;i<3;i++) {
		for (std::size_t j=0;j<3;j++) {
			matrix [i][j] = rotationSubMatrix [i][j];
		}
	}

	return matrix;
}

BufferObject AnimationModel3DRenderer::BindVertexData (const std::vector<AnimatedVertexData>& vBuf, const std::vector<unsigned int>& iBuf)
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

	//numar de indecsi
	std::size_t indexCount = iBuf.size();

	BufferObject bufferObject;
	bufferObject.VAO_INDEX = VAO;
	bufferObject.VBO_INDEX = VBO;
	bufferObject.IBO_INDEX = IBO;
	bufferObject.VBO_INSTANCE_INDEX = 0;
	bufferObject.INDEX_COUNT = indexCount;

	return bufferObject;
}