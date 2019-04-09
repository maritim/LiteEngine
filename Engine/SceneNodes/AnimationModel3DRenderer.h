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
	std::string _currentAnimClipName;
	float _currAnimStartTime;

	std::string _previousAnimClipName;
	float _blendStartTime;
	float _globalBlendStartTime;
	float _blendDuration;

public:
	using Model3DRenderer::Model3DRenderer;	

	void Attach (Model* model);

	void Draw ();
	void DrawGeometry ();

	void SetAnimationClip (const std::string& animName);
	void Blend (const std::string& nextAnimName, float duration);

protected:
	BufferObject ProcessPolygonGroup (Model* model, PolygonGroup* polyGroup);

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void ProcessBoneTransform (AnimationModel* animModel, const glm::mat4& parentTransform,
		AnimationContainer* prevAnimContainer, AnimationContainer* currAnimContainer, BoneNode* boneNode,
		const glm::mat4& inverseGlobalMatrix, float prevAnimTime, float currAnimTime,
		float blendWeight, std::vector<glm::mat4>& boneTransform);

	void CalcInterpolatedScaling (glm::vec3& scaling, float animTime, AnimationNode* animNode);
	void CalcInterpolatedPosition (glm::vec3& position, float animTime, AnimationNode* animNode);
	void CalcInterpolatedRotation (glm::quat& rotation, float animTime, AnimationNode* animNode);

	BufferObject BindVertexData (const std::vector<AnimatedVertexData>& vBuf, const std::vector<unsigned int>& iBuf);
};

#endif