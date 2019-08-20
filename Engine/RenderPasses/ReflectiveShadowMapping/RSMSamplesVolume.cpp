#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "RSMSamplesVolume.h"

#include <cstring>

#include "Wrappers/OpenGL/GL.h"

#include "Core/Random/Random.h"

RSMSamplesVolume::RSMSamplesVolume () :
	_samplesUBO (0)
{
}

bool RSMSamplesVolume::Init (std::size_t samplesCount)
{
	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec2 sample;

		float radius = Random::Instance ()->RangeF (0, 1.0f);
		float angle = Random::Instance ()->RangeF (0, 2 * M_PI);

		sample.x = radius * sin (angle);
		sample.y = radius * cos (angle);

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	return true;
}

void RSMSamplesVolume::BindForReading ()
{
	/*
	 * Nothing
	*/
}

void RSMSamplesVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> RSMSamplesVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmSamples;

	rsmSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	rsmSamples.name = "rsmSamples";

	rsmSamples.value.x = _samplesUBO;

	attributes.push_back (rsmSamples);

	return attributes;
}

std::size_t RSMSamplesVolume::GetSize () const
{
	return _samples.samplesCount;
}

void RSMSamplesVolume::Clear ()
{
	GL::DeleteBuffers (1, &_samplesUBO);
}