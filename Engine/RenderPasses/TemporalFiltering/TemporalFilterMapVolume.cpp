#include "TemporalFilterMapVolume.h"

TemporalFilterMapVolume::TemporalFilterMapVolume () :
	_viewProjectionMatrix (1.0f),
	_current (false)
{

}

void TemporalFilterMapVolume::SetViewProjectionMatrix (const glm::mat4& viewProjectionMatrix)
{
	_viewProjectionMatrix = viewProjectionMatrix;
}

void TemporalFilterMapVolume::SetCurrent (bool current)
{
	_current = current;
}

const glm::mat4& TemporalFilterMapVolume::GetViewProjectionMatrix () const
{
	return _viewProjectionMatrix;
}

std::vector<PipelineAttribute> TemporalFilterMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute temporalFilterMap;

	temporalFilterMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	temporalFilterMap.name = _current == true ? "postProcessMap" : "temporalFilterMap";

	temporalFilterMap.value.x = _colorBuffer;

	attributes.push_back (temporalFilterMap);

	return attributes;
}
