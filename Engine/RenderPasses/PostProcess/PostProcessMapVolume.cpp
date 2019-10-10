#include "PostProcessMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void PostProcessMapVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
}

std::vector<PipelineAttribute> PostProcessMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute postProcessMap;

	postProcessMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	postProcessMap.name = "postProcessMap";

	postProcessMap.value.x = _colorBuffer;

	attributes.push_back (postProcessMap);

	return attributes;
}
