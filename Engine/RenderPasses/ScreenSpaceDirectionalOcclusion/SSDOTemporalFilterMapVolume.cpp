#include "SSDOTemporalFilterMapVolume.h"

std::vector<PipelineAttribute> SSDOTemporalFilterMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssdoMap;

	ssdoMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	ssdoMap.name = _current == true ? "ssdoMap" : "temporalFilterMap";

	ssdoMap.value.x = _colorBuffer;

	attributes.push_back (ssdoMap);

	return attributes;
}
