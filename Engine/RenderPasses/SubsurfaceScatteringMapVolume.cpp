#include "SubsurfaceScatteringMapVolume.h"

std::vector<PipelineAttribute> SubsurfaceScatteringMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute subsurfaceScatteringMap;

	subsurfaceScatteringMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	subsurfaceScatteringMap.name = "subsurfaceScatteringMap";

	subsurfaceScatteringMap.value.x = _colorBuffer;

	attributes.push_back (subsurfaceScatteringMap);

	return attributes;
}
