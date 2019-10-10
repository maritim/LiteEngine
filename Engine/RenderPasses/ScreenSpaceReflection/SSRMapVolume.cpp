#include "SSRMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void SSRMapVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
}

std::vector<PipelineAttribute> SSRMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute reflectionMap;

	reflectionMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	reflectionMap.name = "reflectionMap";

	reflectionMap.value.x = _colorBuffer;

	attributes.push_back (reflectionMap);

	return attributes;
}
