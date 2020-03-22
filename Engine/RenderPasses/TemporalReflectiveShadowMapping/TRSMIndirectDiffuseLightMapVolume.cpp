#include "TRSMIndirectDiffuseLightMapVolume.h"

TRSMIndirectDiffuseLightMapVolume::TRSMIndirectDiffuseLightMapVolume () :
	_viewProjectionMatrix (1.0f),
	_current (true)
{

}

void TRSMIndirectDiffuseLightMapVolume::SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix)
{
	_viewProjectionMatrix = viewProjectionMatrix;
}

void TRSMIndirectDiffuseLightMapVolume::SetCurrent (bool current)
{
	_current = current;
}

const glm::mat4& TRSMIndirectDiffuseLightMapVolume::GetViewProjectionMatrix () const
{
	return _viewProjectionMatrix;
}

std::vector<PipelineAttribute> TRSMIndirectDiffuseLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectDiffuseMap;

	indirectDiffuseMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectDiffuseMap.name = _current == true ? "indirectDiffuseMap" : "lastIndirectDiffuseMap";

	indirectDiffuseMap.value.x = _colorBuffer;

	attributes.push_back (indirectDiffuseMap);

	return attributes;
}
