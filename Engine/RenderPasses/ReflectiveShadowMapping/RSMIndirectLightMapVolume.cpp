#include "RSMIndirectLightMapVolume.h"

RSMIndirectLightMapVolume::RSMIndirectLightMapVolume () :
	_viewProjectionMatrix (1.0f),
	_current (true)
{

}

void RSMIndirectLightMapVolume::SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix)
{
	_viewProjectionMatrix = viewProjectionMatrix;
}

void RSMIndirectLightMapVolume::SetCurrent (bool current)
{
	_current = current;
}

const glm::mat4& RSMIndirectLightMapVolume::GetViewProjectionMatrix () const
{
	return _viewProjectionMatrix;
}

std::vector<PipelineAttribute> RSMIndirectLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectMap;

	indirectMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectMap.name = _current == true ? "indirectMap" : "lastIndirectMap";

	indirectMap.value.x = _colorBuffer;

	attributes.push_back (indirectMap);

	return attributes;
}
