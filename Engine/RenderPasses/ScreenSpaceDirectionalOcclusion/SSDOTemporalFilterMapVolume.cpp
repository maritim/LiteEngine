#include "SSDOTemporalFilterMapVolume.h"

void SSDOTemporalFilterMapVolume::SetCurrent (bool current)
{
	/*
	 * Set current
	*/

	TemporalFilterMapVolume::SetCurrent (current);

	/*
	 * Update attributes
	*/

	_attributes.clear ();

	PipelineAttribute temporalFilterMap;

	temporalFilterMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	temporalFilterMap.name = _current == true ? "ssdoMap" : "temporalFilterMap";

	temporalFilterMap.value.x = _framebufferView->GetTextureView (0)->GetGPUIndex ();

	_attributes.push_back (temporalFilterMap);
}
