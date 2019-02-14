#include "SSRMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void SSRMapVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE15);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

std::vector<PipelineAttribute> SSRMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute reflectionMap;

	reflectionMap.type = PipelineAttribute::AttrType::ATTR_1I;

	reflectionMap.name = "reflectionMap";

	reflectionMap.value.x = 15;

	attributes.push_back (reflectionMap);

	return attributes;
}
