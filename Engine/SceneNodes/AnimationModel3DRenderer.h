#ifndef ANIMATIONMODEL3DRENDERER
#define ANIMATIONMODEL3DRENDERER

#include "Model3DRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Mesh/AnimationModel.h"

#include "Renderer/PipelineAttribute.h"

struct AnimatedVertexData : VertexData
{
	int bones[4];
	float weights[4];

	AnimatedVertexData ();
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

	void ProcessBoneTransform (AnimationModel* animModel, const glm::mat4& parentTransform,
		AnimationContainer* animContainer, BoneNode* boneNode, const glm::mat4& inverseGlobalMatrix,
		float animationTime, std::vector<glm::mat4>& boneTransform);

	void CalcInterpolatedScaling (glm::vec3& scaling, float animTime, AnimationNode* animNode);
	void CalcInterpolatedPosition (glm::vec3& position, float animTime, AnimationNode* animNode);
	void CalcInterpolatedRotation (glm::quat& rotation, float animTime, AnimationNode* animNode);

	BufferObject BindVertexData (const std::vector<AnimatedVertexData>& vBuf, const std::vector<unsigned int>& iBuf);
};

#endif