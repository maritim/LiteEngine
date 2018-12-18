#include "PostProcessMapVolume.h"

#include "Wrappers/OpenGL/GL.h"

void PostProcessMapVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE18);
	GL::BindTexture (GL_TEXTURE_2D, _colorBuffer);
}

std::vector<PipelineAttribute> PostProcessMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute postProcessMap;

	postProcessMap.type = PipelineAttribute::AttrType::ATTR_1I;

	postProcessMap.name = "postProcessMap";

	postProcessMap.value.x = 18;

	attributes.push_back (postProcessMap);

	return attributes;
}
