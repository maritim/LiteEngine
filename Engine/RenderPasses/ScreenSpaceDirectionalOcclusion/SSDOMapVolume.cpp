#include "SSDOMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

void SSDOMapVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE15);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

std::vector<PipelineAttribute> SSDOMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssdoMap;

	ssdoMap.type = PipelineAttribute::AttrType::ATTR_1I;

	ssdoMap.name = "ssdoMap";

	ssdoMap.value.x = 15;

	attributes.push_back (ssdoMap);

	return attributes;
}
