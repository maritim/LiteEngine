#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "SSDOSamplesVolume.h"

#include <cstring>

#include "Core/Random/Random.h"

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Sequences/HaltonGenerator.h"

SSDOSamplesVolume::SSDOSamplesVolume (std::size_t samplesCount) :
	_samplesUBO (0)
{
	/*
	 * Initialize samples
	*/

	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	HaltonGenerator haltonGenerator (2, 3);

	for (std::size_t index = 0; index < samplesCount; index++) {

		glm::vec3 sample (
			std::cos (index * 2 * M_PI / samplesCount),
			std::sin (index * 2 * M_PI / samplesCount),
			haltonGenerator.Next ().x
		);

		/*
		 *
		*/

		sample = glm::normalize (sample);
		sample *= haltonGenerator.Next ().x;

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
		_samples.samples [index * 4 + 2] = sample.z;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	/*
	 * Update attributes
	*/

	PipelineAttribute ssdoSamples;

	ssdoSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	ssdoSamples.name = "ssdoSamples";

	ssdoSamples.value.x = _samplesUBO;

	_attributes.push_back (ssdoSamples);
}

SSDOSamplesVolume::~SSDOSamplesVolume ()
{
	GL::DeleteBuffers (1, &_samplesUBO);
}

const std::vector<PipelineAttribute>& SSDOSamplesVolume::GetCustomAttributes () const
{
	return _attributes;
}

std::size_t SSDOSamplesVolume::GetSamplesCount () const
{
	return _samples.samplesCount;
}
