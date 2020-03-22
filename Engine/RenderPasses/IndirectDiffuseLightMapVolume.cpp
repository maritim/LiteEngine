#include "IndirectDiffuseLightMapVolume.h"

std::vector<PipelineAttribute> IndirectDiffuseLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectDiffuseMap;

	indirectDiffuseMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectDiffuseMap.name = "indirectDiffuseMap";

	indirectDiffuseMap.value.x = _colorBuffer;

	attributes.push_back (indirectDiffuseMap);

	return attributes;
}
