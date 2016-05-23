#ifndef ANIMATIONMODEL3DRENDERER
#define ANIMATIONMODEL3DRENDERER

#include "Model3DRenderer.h"

#include "Mesh/AnimationModel.h"

#include "Renderer/PipelineAttribute.h"

struct AnimatedVertexData : VertexData
{
	unsigned int bones[4];
	float weights[4];
};

class AnimationModel3DRenderer : public Model3DRenderer
{
protected:
	AnimationModel* _animationModel;

public:
	using Model3DRenderer::Model3DRenderer;	

	void Attach (Model* model);

	void Draw ();

protected:
	BufferObject ProcessPolygonGroup (Model* model, PolygonGroup* polyGroup);

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void ProcessBoneTransform (AnimationModel* animModel, aiMatrix4x4 parentTransform,
		AnimationContainer* animContainer, BoneNode* boneNode, aiMatrix4x4 inverseGlobalMatrix,
		float animationTime, std::vector<aiMatrix4x4>& boneTransform);

	void CalcInterpolatedScaling (aiVector3D& scaling, float animTime , AnimationNode* animNode);
	void CalcInterpolatedPosition (aiVector3D& position, float animTime , AnimationNode* animNode);
	void CalcInterpolatedRotation (aiQuaternion& rotation, float animTime , AnimationNode* animNode);

	aiMatrix4x4 GetRotationMatrix (aiQuaternion rotation);

	BufferObject BindVertexData (const std::vector<AnimatedVertexData>& vBuf, const std::vector<unsigned int>& iBuf);
};

#endif