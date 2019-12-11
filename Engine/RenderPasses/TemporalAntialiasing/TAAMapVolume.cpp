#include "TAAMapVolume.h"

TAAMapVolume::TAAMapVolume () :
	_viewProjectionMatrix (1.0f),
	_current (false)
{

}

void TAAMapVolume::SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix)
{
	_viewProjectionMatrix = viewProjectionMatrix;
}

void TAAMapVolume::SetCurrent (bool current)
{
	_current = current;
}

const glm::mat4& TAAMapVolume::GetViewProjectionMatrix () const
{
	return _viewProjectionMatrix;
}

std::vector<PipelineAttribute> TAAMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute taaMap;

	taaMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	taaMap.name = _current == true ? "postProcessMap" : "taaMap";

	taaMap.value.x = _colorBuffer;

	attributes.push_back (taaMap);

	return attributes;
}
