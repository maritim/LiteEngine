#include "RSMIndirectLightMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

std::vector<PipelineAttribute> RSMIndirectLightMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectMap;

	indirectMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	indirectMap.name = "indirectMap";

	indirectMap.value.x = _colorBuffer;

	attributes.push_back (indirectMap);

	return attributes;
}
