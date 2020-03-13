#include "IndirectSpecularLightMapVolume.h"

std::vector<PipelineAttribute> IndirectSpecularLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectMap;

	indirectMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectMap.name = "indirectSpecularMap";

	indirectMap.value.x = _colorBuffer;

	attributes.push_back (indirectMap);

	return attributes;
}
