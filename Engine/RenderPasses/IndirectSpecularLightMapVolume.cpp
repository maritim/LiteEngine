#include "IndirectSpecularLightMapVolume.h"

std::vector<PipelineAttribute> IndirectSpecularLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectSpecularMap;

	indirectSpecularMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectSpecularMap.name = "indirectSpecularMap";

	indirectSpecularMap.value.x = _colorBuffer;

	attributes.push_back (indirectSpecularMap);

	return attributes;
}
