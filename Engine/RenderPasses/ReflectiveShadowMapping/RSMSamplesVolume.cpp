#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "RSMSamplesVolume.h"

#include <cstring>

#include "Wrappers/OpenGL/GL.h"

#include "Core/Random/Random.h"

#include "Utils/Sequences/HaltonGenerator.h"

RSMSamplesVolume::RSMSamplesVolume (std::size_t samplesCount) :
	_samplesUBO (0)
{
	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	HaltonGenerator haltonGenerator (2, 3);

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec2 sample = haltonGenerator.Next () * 2.0f - 1.0f;

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	/*
	 * Update attributes
	*/

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmSamples;

	rsmSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	rsmSamples.name = "rsmSamples";

	rsmSamples.value.x = _samplesUBO;

	_attributes.push_back (rsmSamples);
}

RSMSamplesVolume::~RSMSamplesVolume ()
{
	GL::DeleteBuffers (1, &_samplesUBO);
}

const std::vector<PipelineAttribute>& RSMSamplesVolume::GetCustomAttributes () const
{
	return _attributes;
}

std::size_t RSMSamplesVolume::GetSize () const
{
	return _samples.samplesCount;
}
