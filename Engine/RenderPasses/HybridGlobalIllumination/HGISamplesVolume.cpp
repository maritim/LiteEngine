#include "HGISamplesVolume.h"

HGISamplesVolume::HGISamplesVolume (std::size_t samplesCount) :
	SSDOSamplesVolume (samplesCount)
{
	/*
	 * Update attributes
	*/

	_attributes.clear ();

	PipelineAttribute ssdoSamples;

	ssdoSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	ssdoSamples.name = "hgiSamples";

	ssdoSamples.value.x = _samplesUBO;

	_attributes.push_back (ssdoSamples);
}
