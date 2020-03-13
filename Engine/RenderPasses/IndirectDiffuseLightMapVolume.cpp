#include "IndirectDiffuseLightMapVolume.h"

std::vector<PipelineAttribute> IndirectDiffuseLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectMap;

	indirectMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectMap.name = "indirectDiffuseMap";

	indirectMap.value.x = _colorBuffer;

	attributes.push_back (indirectMap);

	return attributes;
}
