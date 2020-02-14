#include "IndirectLightMapVolume.h"

std::vector<PipelineAttribute> IndirectLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectMap;

	indirectMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectMap.name = "indirectMap";

	indirectMap.value.x = _colorBuffer;

	attributes.push_back (indirectMap);

	return attributes;
}
