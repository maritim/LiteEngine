#include "SSDOMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

void SSDOMapVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
}

std::vector<PipelineAttribute> SSDOMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssdoMap;

	ssdoMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	ssdoMap.name = "ssdoMap";

	ssdoMap.value.x = _colorBuffer;

	attributes.push_back (ssdoMap);

	return attributes;
}
