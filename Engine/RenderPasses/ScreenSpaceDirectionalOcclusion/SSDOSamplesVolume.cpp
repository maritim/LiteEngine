#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "SSDOSamplesVolume.h"

#include <cstring>

#include "Core/Random/Random.h"

#include "Wrappers/OpenGL/GL.h"

SSDOSamplesVolume::SSDOSamplesVolume () :
	_samplesUBO (0)
{

}

bool SSDOSamplesVolume::Init (std::size_t samplesCount)
{
	/*
	 * Initialize samples
	*/

	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec3 sample (
			std::cos (index * 2 * M_PI / samplesCount),
			std::sin (index * 2 * M_PI / samplesCount),
			Random::Instance ()->RangeF (0.0f, 1.0f)
		);

		/*
		 *
		*/

		sample = glm::normalize (sample);
		sample *= Random::Instance ()->RangeF (0.5f, 1.0f);

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
		_samples.samples [index * 4 + 2] = sample.z;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	return true;
}

void SSDOSamplesVolume::BindForReading ()
{
	/*
	 * Nothing
	*/
}

void SSDOSamplesVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> SSDOSamplesVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssdoSamples;

	ssdoSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	ssdoSamples.name = "ssdoSamples";

	ssdoSamples.value.x = _samplesUBO;

	attributes.push_back (ssdoSamples);

	return attributes;
}

std::size_t SSDOSamplesVolume::GetSamplesCount () const
{
	return _samples.samplesCount;
}

void SSDOSamplesVolume::Clear ()
{
	GL::DeleteBuffers (1, &_samplesUBO);
}